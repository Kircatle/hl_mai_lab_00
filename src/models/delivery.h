#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"
#include "Poco/DateTime.h"

namespace models
{
    class Delivery
    {
        private:
            std::string delivery_uuid;
            std::string package_uuid;
            std::string sender_uuid;
            std::string receiver_uuid;
            std::string departure_date;
            std::string start_point;
            std::string destination;
            std::string status;
        public:
            static void init();
            static Delivery from_json(const std::string &json);

            static std::optional<Delivery> get_by_id(std::string &id);
            static std::vector<Delivery> get_by_package_uuid(std::string &package_uuid);
            static std::vector<Delivery> get_by_sender_uuid(std::string &sender_uuid);
            static std::vector<Delivery> get_by_receiver_uuid(std::string &receiver_uuid);
            Poco::JSON::Object::Ptr to_json() const;
            void save_to_db();


            std::string get_delivery_uuid() const;
            std::string get_package_uuid() const;
            std::string get_sender_uuid() const;
            std::string get_receiver_uuid() const;
            std::string get_departure_date() const;
            std::string get_start_point() const;
            std::string get_destination() const;
            std::string get_status() const;

            void set_package_uuid(std::string pkg_uuid);
            void set_sender_uuid(std::string send_uuid);
            void set_receiver_uuid(std::string rcv_uuid);
            void set_departure_date(std::string dpt_date);
            void set_start_point(std::string sp);
            void set_destination(std::string dest);
            void set_status(std::string status);
    };
}

#endif