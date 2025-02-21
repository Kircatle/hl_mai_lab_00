#ifndef USER_HANDLER_H
#define USER_HANDLER_H

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

#include "../../models/user.h"
#include "../../helpers/send_not_found_exception.h"
#include "../../helpers/identity_helper.h"
#include "../../helpers/send_message.h"
class UserHandler : public HTTPRequestHandler
{
  private:

    bool check_login(std::string &login, std::string &reason)
    {

        if (isdigit(login[0]))
        {
            reason = "Login first sign can't be digit";
            return false;
        }
        std::optional<models::User> user = models::User::get_by_login(login);
        if (user)
        {
            reason = "A user with this login already exists";
            return false;
        }

        return true;
    };

    bool check_name(const std::string &name, std::string &reason)
    {
        if (name.length() < 3)
        {
            reason = "Name must be at leas 3 signs";
            return false;
        }

        if (name.find(' ') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        if (name.find('\t') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        return true;
    };

    bool check_email(std::string &email, std::string &reason)
    {   
        reason = "";
        if (email.find('@') == std::string::npos)
        {
            reason = "Email must contain @";
            return false;
        }

        if (email.find(' ') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        if (email.find('\t') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }
        std::optional<models::User> user = models::User::get_by_email(email);
        if (user)
        {
            reason = "A user with this email already exists";
            return false;
        }

        return true;
    };

    public:
        UserHandler(const std::string &format) : _format(format) {}
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
            try 
            {
                if ((form.has("id") || form.has("login"))  && path == "/user" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    if (form.has("id"))
                    {
                        bool use_cache = request.get("Cache-Control", "no-cache") == "cache";
                        std::string user_uuid = form.get("id").c_str();
                        if (!use_cache)
                        {
                            std::optional<models::User> result = models::User::get_by_id(user_uuid);
                            if (result)
                            {
                                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                std::ostream &ostr = response.send();
                                Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                            }
                            else
                            {
                                send_not_found_exception("User with id " + user_uuid + " was not found", "/user", response);
                            } 
                            return;
                        }
                        else
                        {
                           std::optional<models::User> result = models::User::from_cache(user_uuid);
                           if (result)
                           {
                                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                std::ostream &ostr = response.send();
                                Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                                return;
                           }
                           else
                           {
                                std::optional<models::User> result = models::User::get_by_id(user_uuid);
                                if (result)
                                {
                                    result->save_to_cache(user_uuid);
                                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                    std::ostream &ostr = response.send();
                                    Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                                }
                                else
                                {
                                    send_not_found_exception("User with id " + user_uuid + " was not found", "/user", response);
                                } 
                                return;
                           }
                        } 
                        
                    }
                    else if (form.has("login"))
                    {
                        bool use_cache = request.get("Cache-Control", "no-cache") == "cache";
                        std::string login = form.get("login").c_str();
                       
                        if (!use_cache)
                        {
                            std::optional<models::User> result = models::User::get_by_login(login);
                            if (result)
                            {
                                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                std::ostream &ostr = response.send();
                                Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                            }
                            else
                            {
                                send_not_found_exception("User with login " + login + " was not found", "/user", response);
                            } 
                            return;
                        }
                        else
                        {
                            std::optional<models::User> result = models::User::from_cache(login);
                            if (result)
                            {
                                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                std::ostream &ostr = response.send();
                                Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                            }
                            else
                            {
                                std::optional<models::User> result = models::User::get_by_login(login);
                                if (result)
                                {
                                    result->save_to_cache(login);
                                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                                    std::ostream &ostr = response.send();
                                    Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                                }
                                else
                                {
                                    send_not_found_exception("User with login " + login + " was not found", "/user", response);
                                } 
                                return;
                            } 
                            return;
                        }
                    }
                }
                else if (path == "/user" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    send_not_found_exception("Please specify id or login","/user", response);
                }
                else if (path == "/user" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    models::User user;
                    Poco::Nullable<std::string> user_id;
                    std::string first_name = "";
                    std::string last_name = "";
                    std::string login = "";
                    std::string password = "";
                    std::string email = "";
                    std::string reason = "";
                    std::string message = "";
                    Poco::Nullable<std::string> title;
                    bool check_result = true;
                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        user_id = object->getNullableValue<std::string>("id");
                        first_name = object->getValue<std::string>("first_name");
                        last_name = object->getValue<std::string>("last_name");
                        login = object->getValue<std::string>("login");
                        password = object->getValue<std::string>("password");
                        email = object->getValue<std::string>("email");
                        title = object->getNullableValue<std::string>("title");
                        if (!check_login(login,reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                        if (!check_name(first_name,reason)) 
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                        if (!check_name(last_name,reason)) 
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }
                        if (!check_email(email,reason)) 
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
                        if (!user_id.isNull()) user.set_user_uuid(user_id.value());
                        user.set_first_name(first_name);
                        user.set_last_name(last_name);
                        user.set_login(login);
                        user.set_password(password);
                        user.set_email(email);
                        if (!title.isNull()) user.set_title(title.value());

                        user.send_to_queue();
                        std::string message = "sended to queue!";
                        send_message(message, response);
                        return;
                    }
                    else
                    {
                        send_not_found_exception(message, "/user/auth", response);
                        return;
                    }
                   
                }

                else if (path == "/user/auth" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    try
                    {

                        std::string login;
                        std::string password;
                        std::string scheme;
                        std::string info;
                        request.getCredentials(scheme, info);
                        get_identity(info,login,password);
                        std::optional<models::User> result;
                        std::cout << login << password;
                        if (login.empty())
                        {
                            send_not_found_exception("Missing login", "/user/auth", response);
                            return;
                        }
                        else if (password.empty())
                        {
                            send_not_found_exception("Missing password", "/user/auth", response);
                            return;
                        }
                    
                        if (!login.empty())
                        {
                            result = models::User::auth_by_login(login, password);
                        }
                        else
                        {
                            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_ACCEPTABLE);
                            Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                            json->set("title", "Internal exception");
                            json->set("status", "406");
                            json->set("detail", "Unallowes authorize method");
                            json->set("instance", "/user");
                            std::ostream &ostr = response.send();
                            Poco::JSON::Stringifier::stringify(json, ostr);
                            return;
                        }
                        
                        if (result)
                        {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            std::ostream &ostr = response.send();
                            ostr << "{ \"id\" : \"" << result->get_user_uuid() << "\"}" << std::endl;
                            return;
                            // Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                        }
                        else
                        {
                            send_not_found_exception("User not found", "/user", response);
                            return;
                        }
                    }
                    catch (std::exception &e){
                            std::cout << "Error auth" << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                            return;
                    }
                }
                else if (path == "/user/find" && request.getMethod() == HTTPRequest::HTTP_GET && (form.has("login") || form.has("email") || (form.has("first_name") && form.has("last_name"))))
                {
                    std::string email = form.get("email", "");
                    std::string login = form.get("login", "");
                    std::string first_name = form.get("first_name", "");
                    std::string last_name = form.get("last_name", "");
                    std::vector<models::User> result;
                    if (!login.empty())
                    {
                        try
                        {
                           result = models::User::find_by_login(login);
                        } catch (std::exception &e) {
                            std::cout << "Error find user" << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else if (!email.empty())
                    {
                        try
                        {
                            result = models::User::find_by_email(email);
                        } catch (std::exception &e) {
                            std::cout << "Error find user" << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else if (!first_name.empty() && !last_name.empty())
                    {
                        try
                        {
                            result = models::User::find_by_first_last_name(first_name, last_name);
                        } catch (std::exception &e) {
                            std::cout << "Error find user" << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_ACCEPTABLE);
                        Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                        json->set("title", "Internal exception");
                        json->set("status", "406");
                        json->set("detail", "Unallowes find method");
                        json->set("instance", "/user");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(json, ostr);
                        return;
                    }

                   if (!result.empty())
                    {
                        Poco::JSON::Array output;
                        for (models::User user : result)
                            output.add(user.to_json());

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(output, ostr);
                        return;
                    }
                    else
                    {
                        send_not_found_exception("User not found", "/user", response);
                    }
                }
            }
            catch (...) {}

            send_not_found_exception("Request receiver with path: " + path + " not found", "", response);
            return;
        }

    private:
        std::string _format;

};

#endif