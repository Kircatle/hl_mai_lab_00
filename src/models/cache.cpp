#include "cache.h"
#include "../config/config.h"

#include <exception>
#include <mutex>

#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>

namespace database
{
    std::mutex mutex;
    Cache::Cache()
    {
        Config config = Config::get_instanse();
        std::string cache_server = config.get_cache_server();

        std::string host = cache_server.substr(0, cache_server.find(':'));
        std::string port = cache_server.substr(cache_server.find(':') + 1);
        std::cout << "Cache host:" << host <<" port: " << port << std::endl;
        stream = rediscpp::make_stream(host, port);
    }

    Cache Cache::get_instance()
    {
        static Cache instance;
        return instance;
    }

    void Cache::put([[maybe_unused]] std::string id, [[maybe_unused]] const std::string &value)
    {
        std::lock_guard<std::mutex> lck(mutex);
        rediscpp::value response = rediscpp::execute(
            *stream, "set", id, value, "ex", "60"
        );
    }

    bool Cache::get([[maybe_unused]] std::string id, [[maybe_unused]] std::string &value)
    {
        std::lock_guard<std::mutex> lck(mutex);
        rediscpp::value response = rediscpp::execute(*stream, "get",id);

        if (response.is_error_message() || response.empty()) return false;
        std::string str = response.as_string().data();
        std::cout << "Redis response: " + str << std::endl;

        value = response.as<std::string>();
        return true;
    }
}