#ifndef USER_H
#define USER_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"

namespace models
{
    class User
    {
        private:
            std::string user_uuid;
            std::string login;
            std::string password;
            std::string first_name;
            std::string last_name;
            std::string email;
            std::string title;

        public:
            static void init();
            static User from_json(std::string &json);
            static std::optional<User> from_cache(const std::string &id);

            static std::optional<User> get_by_id(std::string &id);
            static std::optional<User> get_by_login(std::string &login);
            static std::optional<User> get_by_email(std::string &login);
            static std::optional<User> auth_by_login(std::string &login, std::string &password);
            static std::optional<User> auth_by_email(std::string &email, std::string &password);
            static std::vector<User>  find_by_login(std::string &login);
            static std::vector<User>  find_by_email(std::string &email);
            static std::vector<User> find_by_first_last_name(std::string &first_name, std::string &last_name);
            
            long table_size();
            Poco::JSON::Object::Ptr to_json() const;
            void save_to_db();
            void save_to_cache(std::string &key);
            std::string get_user_uuid() const;
            std::string get_login() const;
            std::string get_password() const;
            std::string get_first_name() const;
            std::string get_last_name() const;
            std::string get_email() const;
            std::string get_title() const;
            void set_user_uuid(std::string id);
            void set_login(std::string f_n);
            void set_password(std::string l_n);
            void set_first_name(std::string log);
            void set_last_name(std::string pswrd);
            void set_email(std::string mail);
            void set_title(std::string p_n);
            void send_to_queue();
    };
}

#endif