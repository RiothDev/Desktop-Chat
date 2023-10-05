#include <iostream>
#include <memory>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Server {
    private:
    std::unique_ptr<int> port;
    std::unique_ptr<int> connections;
    SOCKET serverSocket;
    SOCKET clientSocket;

    public:
    Server(const int p, const int c) : port(std::make_unique<int>(p)), connections(std::make_unique<int>(c)), serverSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET) {}
    ~Server() { cleanup(); }

    void closeConnection(const std::string&& err) {
        std::cerr << err << std::endl;

        cleanup();
        exit(1);
    }

    void cleanup() {
        if(clientSocket != INVALID_SOCKET) closesocket(clientSocket);
        if(serverSocket != INVALID_SOCKET) closesocket(serverSocket);

        WSACleanup();
    }

    void startServer() {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Error trying to start Winsock." << std::endl;
            exit(1);
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(serverSocket == INVALID_SOCKET) {
            std::cerr << "Error trying to create server socket." << std::endl;
            WSACleanup();

            exit(1);
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(*port);

        if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR || listen(serverSocket, *connections) == SOCKET_ERROR) {
            closeConnection("Error trying to bind socket server.");
        }

        std::cout << "> Server started..." << std::endl;

        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if(clientSocket == INVALID_SOCKET) {
            closeConnection("Error trying to start connection.");
        }

        char buffer[1024];

        while(true) {
            memset(buffer, 0, sizeof(buffer));

            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

            if(bytesRead <= 0) {
                std::cerr << "Closing the connection..." << std::endl;
                break;
            }

            std::cout << "Client message: " << buffer << std::endl;
        }

        closeConnection("The connection has been closed.");
    }
};