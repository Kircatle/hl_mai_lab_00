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
    if (std::getenv("URL_USER_SERVICE") == NULL)
    {
        _url_user_service = "";
    }
    else
    {
        _url_user_service = std::getenv("URL_USER_SERVICE");
    }
    if (std::getenv("APP_PORT")==NULL)
    {
        _application_port = "";
    }
    else
    {
        _application_port = std::getenv("APP_PORT");
    }
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

const std::string &Config::get_url_user_service()
{
    return _url_user_service;
}

const std::string &Config::get_application_port()
{
    return _url_user_service;
}