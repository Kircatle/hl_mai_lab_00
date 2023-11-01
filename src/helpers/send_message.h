#ifndef SEND_MESSAGE_H
#define SEND_MESSAGE_H

#include <string>
#include "Poco/Net/HTTPServerResponse.h"
#include <Poco/JSON/Object.h>


void send_message(std::string message, Poco::Net::HTTPServerResponse &response)
{
    std::cout << message << std::endl;
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
    json->set("message", message);
    std::ostream &ostr = response.send();
    Poco::JSON::Stringifier::stringify(json, ostr);
}

#endif