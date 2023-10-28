#include "server/web_server.h"

int main(int argc, char* argv[]) {
    WebServer server;
    return server.run(argc, argv);
}