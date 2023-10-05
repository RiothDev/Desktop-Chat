#include <iostream>
#include <memory>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Client {
    private:
    SOCKET clientSocket;
    std::unique_ptr<std::string> ip;
    std::unique_ptr<int> port;

    public:
    Client(const std::string& i, int p) : clientSocket(INVALID_SOCKET), ip(std::make_unique<std::string>(i)), port(std::make_unique<int>(p)) {}
    ~Client() { cleanup(); }

    void closeConnection(const std::string&& err) {
        std::cerr << err << std::endl;

        cleanup();
        exit(1);
    }

    void cleanup() {
        if(clientSocket != INVALID_SOCKET) closesocket(clientSocket);

        WSACleanup();
    }

    void startClient() {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Error trying to start Winsock." << std::endl;
            exit(1);
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(clientSocket == INVALID_SOCKET) {
            std::cerr << "Error trying to create client socket." << std::endl;

            WSACleanup();
            exit(1);
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr((*ip).c_str());
        serverAddress.sin_port = htons(*port);

        if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            closeConnection("Error trying to connect to server");
        }

        while(true) {
            Sleep(100);
            std::cout << "> ";

            std::string message;
            std::getline(std::cin, message);

            send(clientSocket, message.c_str(), message.size(), 0);
        }

        closeConnection("The connection has been closed.");
    }
};