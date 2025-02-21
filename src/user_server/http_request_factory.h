#ifndef HTTP_REQUEST_FACTORY_H
#define HTTP_REQUEST_FACTORY_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
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
#include <iostream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "handlers/user_handler.h"
// #include "handlers/package_handler.h"
// #include "handlers/delivery_handler.h"
class HTTPRequestFactory : public HTTPRequestHandlerFactory
{
    public:
        HTTPRequestFactory(const std::string &format) : _format(format) {}

        HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request)
        {
            std::cout << "Request URI: " << request.getURI() << std::endl;
            if (request.getURI().find("/user") != std::string::npos)
                return new UserHandler(_format);
            // else if (request.getURI().find("/package") != std::string::npos)
            //     return new PackageHandler(_format);
            // else if (request.getURI().find("/delivery") != std::string::npos)
            //     return new DeliveryHandler(_format);
            return 0;
        }

    private:
        std::string _format;
};
#endif