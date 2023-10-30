#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <iostream>
#include <memory>

namespace database
{
    class Cache
    {
        private:
            std::shared_ptr<std::iostream> stream;
            Cache();

        public:
            static Cache get_instance();
            void put(std::string id, const std::string& value);
            bool get(std::string id, std::string& value);
    };
}

#endif