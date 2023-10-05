#include "Server.hpp"

int main(int argc, char* argv[]) {
    int port;

    std::cout << "Port: ";
    std::cin >> port;

    Server server(port, 5);
    server.startServer();

    return 0;
}