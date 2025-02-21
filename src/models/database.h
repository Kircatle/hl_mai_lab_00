#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <Poco/Data/SessionPool.h>

namespace database
{
    class Database
    {
        private:
            std::string connection;
            std::unique_ptr<Poco::Data::SessionPool> pool;
            Database();

        public:
            static Database &get_instance();

            Poco::Data::Session create_database_session();
            static size_t get_max_shard();
            static std::string get_sharding_hint(long hash);
            static std::vector<std::string> get_all_sharding_hints();
    };
}

#endif