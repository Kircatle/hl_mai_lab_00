#include <iostream>
#include <functional>
#include <string>


inline long get_hash(const std::string& str) {
    std::hash<std::string> hasher;
    long hashed = hasher(str);
    return hashed;
}