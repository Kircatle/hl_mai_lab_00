#include <iostream>
#include <functional> //for std::hash
#include <string>

int main() {
    std::string str = "Hello World";
    std::hash<std::string> hasher;
    long hashed = hasher(str); //returns std::size_t
    std::cout << hashed << '\n'; //outputs 2146989006636459346 on my machine
}
