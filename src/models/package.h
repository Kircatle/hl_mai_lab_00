#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"

namespace models
{
    class Package
    {
        private:
            std::string package_uuid;
            std::string user_uuid;
            std::string title;
            double width;
            double height;
            double length;
            double weight;
            std::string description;

        public:
            static void init();
            static Package from_json(const std::string &json);

            static std::optional<Package> get_by_id(std::string &id);
            static std::vector<Package> get_by_user_uuid(std::string &user_uuid);
            Poco::JSON::Object::Ptr to_json() const;
            void save_to_db();
            std::string get_package_uuid() const;
            std::string get_user_uuid() const;
            std::string get_title() const;
            double get_width() const;
            double get_height() const;
            double get_length() const;
            double get_weight() const;
            std::string get_description() const;

            void set_user_uuid(std::string uid);
            void set_title(std::string tl);
            void set_width(double wid);
            void set_height(double hgt);
            void set_length(double lgt);
            void set_weight(double wgt);
            void set_description(std::string descr);
    };
}

#endif