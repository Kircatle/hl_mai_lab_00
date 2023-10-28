#include <stdlib.h>
#include <stdio.h>
#include "config.h"

Config::Config()
{
    _host = std::getenv("DATABASE_HOST");
    _port = std::getenv("DATABASE_PORT");
    _login = std::getenv("DATABASE_LOGIN");
    _password = std::getenv("DATABASE_PASSWORD");
    _database = std::getenv("DATABASE_NAME");
}

Config &Config::get_instanse()
{
    static Config instance;
    return instance;
}

const std::string &Config::get_host()
{
    return _host;
}

const std::string &Config::get_port()
{
    return _port;
}

const std::string &Config::get_login()
{
    return _login;
}

const std::string &Config::get_password()
{
    return _password;
}

const std::string &Config::get_database_path()
{
    return _database;
}