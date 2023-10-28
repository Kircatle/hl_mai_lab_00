#ifndef SEND_UNAUTHORIZED_EXCEPTION_H
#define SEND_UNAUTHORIZED_EXCEPTION_H

#include <string>
#include "Poco/Net/HTTPServerResponse.h"
#include <Poco/JSON/Object.h>

void send_unauthorized_exception(std::string message, Poco::Net::HTTPServerResponse &response)
{
    std::cout << message << std::endl;
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
    json->set("type", "/errors/unauthorized");
    json->set("title", "Unauthorized");
    json->set("status", "401");
    json->set("detail", "invalid login or password");
    json->set("instance", "/use");
    std::ostream &ostr = response.send();
    Poco::JSON::Stringifier::stringify(json, ostr);
}

#endif