#ifndef DELIVERY_HANDLER_H
#define DELIVERY_HANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/JSON/Parser.h"
#include "Poco/URI.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include "Poco/StreamCopier.h"

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPRequest;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../models/delivery.h"
#include "../../helpers/send_not_found_exception.h"
#include "../../models/package.h"
#include "../../config/config.h"



class DeliveryHandler : public HTTPRequestHandler
{
  private:

    bool check_user_uuid(std::string &user_uuid, std::string &reason)
    {
        std::string string_result;
        Config &config = Config::get_instanse();

        try
        {
            
            std::string url = "http://"+config.get_url_user_service()+"/user?id="+user_uuid;
            Poco::URI uri(url);
            Poco::Net::HTTPClientSession s(uri.getHost(), uri.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
            request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
            request.setContentType("application/json");
            request.set("Accept", "application/json");
            request.setKeepAlive(true);

            s.sendRequest(request);

            Poco::Net::HTTPResponse response;
            std::istream &rs = s.receiveResponse(response);

            while (rs)
            {
                char c{};
                rs.read(&c, 1);
                if (rs)
                    string_result += c;
            }
            if (response.getStatus() != 200)
            {
                reason = "User "+user_uuid+" not found";
                return false;
            }
        }
        catch (Poco::Exception &ex)
        {
            reason = "Error send auth request";
            std::cout << "exception:" << ex.what() << std::endl;
            return false;
        }

        return true;
    };
    bool check_package_uuid(std::string &package_uuid, std::string &reason)
    {
        std::optional<models::Package> package = models::Package::get_by_id(package_uuid);
        if (package)
        {
            return true;
        }
        reason = "Package not found";
        return false;
    };

    std::optional<std::string> auth(const std::string &url, const std::string &login, const std::string &password)
    {
        std::string string_result;
        try
        {
            std::string token = login + ":" + password;
            std::ostringstream os;
            Poco::Base64Encoder b64in(os);
            b64in << token;
            b64in.close();
            std::string identity = "Basic " + os.str();

            Poco::URI uri(url);
            Poco::Net::HTTPClientSession s(uri.getHost(), uri.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
            request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
            request.setContentType("application/json");
            request.set("Authorization", identity);
            request.set("Accept", "application/json");
            request.setKeepAlive(true);

            s.sendRequest(request);

            Poco::Net::HTTPResponse response;
            std::istream &rs = s.receiveResponse(response);

            while (rs)
            {
                char c{};
                rs.read(&c, 1);
                if (rs)
                    string_result += c;
            }

            if (response.getStatus() != 200)
                return {};
        }
        catch (Poco::Exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl;
            return std::optional<std::string>();
        }

        return string_result;
    }

    public:
        DeliveryHandler(const std::string &format) : _format(format) {}
        void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
        {
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::URI uri = Poco::URI(request.getURI());
            std::string path = uri.getPath();
            std::cout << "Path of user request: " + path + " " << request.getMethod() << std::endl;
            std::istream& stream = request.stream();
            std::ostringstream oss;
            Poco::StreamCopier::copyStream(stream, oss);
            std::string requestBody = oss.str();
            std::cout << "Request body = " + requestBody << std::endl;
            HTMLForm form(request,stream);
            std::string scheme;
            std::string info;
            std::string login, password;
            request.getCredentials(scheme, info);
            if (scheme == "Basic")
            {
                get_identity(info, login, password);
                std::cout << "login:" << login << std::endl;
                std::cout << "password:" << password << std::endl;
                std::string host = "localhost";
                std::string url;
                url = "http://" + host + ":5055/user/auth";
                if (!auth(url,login,password))
                {
                    send_unauthorized_exception("unath", response);
                    return;
                }
            }
            else
            {
                send_unauthorized_exception("unath", response);
                return; 
            }
            try 
            {
                if (path == "/delivery" && form.has("id") && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                        std::string delivery_uuid = form.get("id").c_str();
                        std::optional<models::Delivery> result = models::Delivery::get_by_id(delivery_uuid);
                        if (result)
                        {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            std::ostream &ostr = response.send();
                            Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                        }
                        else
                        {
                            send_not_found_exception("Delivery with id " + delivery_uuid + " was not found", "/delivery", response);
                            return;
                        } 
                        return;
                }
                else if (path == "/delivery" && form.has("sender_id") && request.getMethod() == HTTPRequest::HTTP_GET)
                {

                    std::string sender_id = form.get("sender_id").c_str();
                    bool check_result = true;
                    std::string message = "";
                    std::string reason = "";

                    if (!check_user_uuid(sender_id,reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    if (!check_result)
                    {
                        send_not_found_exception(message, "/delivery", response);
                        return;
                    }

                    std::vector<models::Delivery> result = models::Delivery::get_by_sender_uuid(sender_id);
                    if (!result.empty())
                    {
                        Poco::JSON::Array output;
                        for (models::Delivery delivery : result)
                            output.add(delivery.to_json());
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(output, ostr);
                        return;
                    }
                    else
                    {
                        send_not_found_exception("Sender have not deliveries", "/delivery", response);
                        return;
                    }
                }
                else if (path == "/delivery" && form.has("receiver_id") && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    std::string receiver_id = form.get("receiver_id").c_str();
                    bool check_result = true;
                    std::string message = "";
                    std::string reason = "";

                    if (!check_user_uuid(receiver_id,reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    if (!check_result)
                    {
                        send_not_found_exception(message,"/delivery", response);
                        return;
                    }
                    std::vector<models::Delivery> result = models::Delivery::get_by_receiver_uuid(receiver_id);
                    if (!result.empty())
                    {
                        Poco::JSON::Array output;
                        for (models::Delivery delivery : result)
                            output.add(delivery.to_json());
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(output, ostr);
                        return;
                    }
                    else
                    {
                        send_not_found_exception(message, "/delivery", response);
                        return;
                    }
                }
                else if (path == "/delivery" && form.has("package_id") && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    std::string package_id = form.get("package_id").c_str();
                    bool check_result = true;
                    std::string message = "";
                    std::string reason = "";

                    if (!check_package_uuid(package_id,reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    if (!check_result)
                    {
                        send_not_found_exception(message,"/delivery", response);
                        return;
                    }
                    std::vector<models::Delivery> result = models::Delivery::get_by_package_uuid(package_id);
                    if (!result.empty())
                    {
                        Poco::JSON::Array output;
                        for (models::Delivery delivery : result)
                            output.add(delivery.to_json());
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(output, ostr);
                        return;
                    }
                    else
                    {
                        send_not_found_exception(message, "/delivery", response);
                        return;
                    }
                }
                else if (path == "/delivery" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    send_not_found_exception("Please specify id or sender_id or receiver_id or package_id","/delivery", response);
                    return;
                }
                else if (path == "/delivery" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    models::Delivery delivery;
                    std::string package_uuid;
                    std::string sender_uuid;
                    std::string receiver_uuid;
                    Poco::Nullable<std::string> departure_date;
                    Poco::Nullable<std::string> start_point;
                    Poco::Nullable<std::string> destination;
                    Poco::Nullable<std::string> status;
                    bool check_result = true;
                    std::string reason;
                    std::string message;
                    
                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        package_uuid = object->getValue<std::string>("package_id");
                        sender_uuid = object->getValue<std::string>("sender_id");
                        receiver_uuid = object->getValue<std::string>("receiver_id");
                        departure_date = object->getNullableValue<std::string>("departure_date");
                        start_point = object->getNullableValue<std::string>("start_point");
                        destination = object->getNullableValue<std::string>("destination");
                        status = object->getNullableValue<std::string>("status");
                        if (!check_user_uuid(sender_uuid,reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                        if (!check_user_uuid(receiver_uuid,reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                        if (!check_package_uuid(package_uuid,reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                    } catch (std::exception &e) {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        return;
                    }
                    if (check_result)
                    {
                        delivery.set_package_uuid(package_uuid);
                        delivery.set_sender_uuid(sender_uuid);
                        delivery.set_receiver_uuid(receiver_uuid);
                        if (!departure_date.isNull()) delivery.set_departure_date(departure_date.value());
                        if (!start_point.isNull()) delivery.set_start_point(start_point.value());
                        if (!destination.isNull()) delivery.set_destination(destination.value());
                        if (!status.isNull()) delivery.set_status(status.value());

                        delivery.save_to_db();
                        std::string delivery_uuid = delivery.get_delivery_uuid();
                        std::cout << delivery_uuid;
                        std::optional<models::Delivery> result = models::Delivery::get_by_id(delivery_uuid);
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
            catch (...) {
                std::cout << "exception";
            }

            send_not_found_exception("Request receiver with path: " + path + " not found", "", response);
        }

    private:
        std::string _format;

};

#endif