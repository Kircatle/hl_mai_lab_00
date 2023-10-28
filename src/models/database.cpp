#include "database.h"
#include "../config/config.h"

namespace database
{
    Database::Database()
    {
        Config &config = Config::get_instanse();

        connection += "host=";
        connection += config.get_host();

        connection += ";port=";
        connection += config.get_port();

        connection += ";user=";
        connection += config.get_login();

        connection += ";password=";
        connection += config.get_password();

        connection += ";db=";
        connection += config.get_database_path();

        std::cout << "Database connection parameters:" << connection << std::endl;

        Poco::Data::MySQL::Connector::registerConnector();
        pool = std::make_unique<Poco::Data::SessionPool>(Poco::Data::MySQL::Connector::KEY, connection);
    }

    Database &Database::get_instance() {
        static Database instance;
        return instance;
    }

    Poco::Data::Session Database::create_database_session()
    {
        return Poco::Data::Session(pool->get());
    }
}