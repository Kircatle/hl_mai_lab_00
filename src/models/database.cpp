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
     size_t Database::get_max_shard(){
        return 2;
    }

    std::string Database::get_sharding_hint(long hash){
        size_t shard_number = hash % get_max_shard();

        std::string result = "-- sharding:";
        result += std::to_string(shard_number);
        return result;
    }

    std::vector<std::string> Database::get_all_sharding_hints() {
        std::vector<std::string> res;
        size_t max_shards = get_max_shard();

        res.reserve(max_shards);
        for (size_t i = 0; i < max_shards; ++i) {
            res.push_back(get_sharding_hint(i));
        }

    return res;
    }

    Poco::Data::Session Database::create_database_session()
    {
        return Poco::Data::Session(pool->get());
    }
}