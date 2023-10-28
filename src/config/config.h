#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

class Config
{
    private:
        Config();
        std::string _host;
        std::string _port;
        std::string _login;
        std::string _password;
        std::string _database;

    public:
        static Config &get_instanse();

        const std::string &get_port();
        const std::string &get_host();
        const std::string &get_login();
        const std::string &get_password();
        const std::string &get_database_path();
};

#endif