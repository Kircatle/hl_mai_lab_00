#ifndef SEND_NOT_FOUND_EXCEPTION_H
#define SEND_NOT_FOUND_EXCEPTION_H

#include <string>
#include "Poco/Net/HTTPServerResponse.h"
#include <Poco/JSON/Object.h>

void send_not_found_exception(std::string message, std::string instance, Poco::Net::HTTPServerResponse &response)
{
    std::cout << message << std::endl;
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
    json->set("type", "/errors/not_found");
    json->set("title", "Internal exception");
    json->set("status", "404");
    json->set("detail", message);
    json->set("instance", instance);
    std::ostream &ostr = response.send();
    Poco::JSON::Stringifier::stringify(json, ostr);
}

#endif