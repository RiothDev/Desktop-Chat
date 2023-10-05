#include "Client.hpp"

int main(int argc, char* argv[]) {
    std::string ip;

    std::cout << "IP: ";
    std::cin >> ip;

    int port;

    std::cout << "Port: ";
    std::cin >> port;

    Client myClient(ip, port);
    myClient.startClient();

    return 0;
}