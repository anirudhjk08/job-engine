#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class TcpServer {
private:
    SOCKET serverSocket = INVALID_SOCKET;

public:

    bool start(int port) {

        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return false;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed\n";
            WSACleanup();
            return false;
        }

        sockaddr_in serverAddress{};

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(
                serverSocket,
                reinterpret_cast<sockaddr*>(&serverAddress),
                sizeof(serverAddress)
            ) == SOCKET_ERROR) {

            std::cerr << "Bind failed\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {

            std::cerr << "Listen failed\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Server listening on port "
                  << port << '\n';

        return true;
    }

    SOCKET acceptClient() {

        sockaddr_in clientAddress{};
        int clientAddressSize = sizeof(clientAddress);

        SOCKET clientSocket = accept(
            serverSocket,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientAddressSize
        );

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
        }

        return clientSocket;
    }

    std::string receiveMessage(SOCKET clientSocket) {

        char buffer[1024]{};

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0) {
            return "";
        }

        return std::string(buffer);
    }

    void sendResponse(
        SOCKET clientSocket,
        const std::string& response
    ) {

        send(
            clientSocket,
            response.c_str(),
            static_cast<int>(response.size()),
            0
        );
    }

    void closeClient(SOCKET clientSocket) {

        closesocket(clientSocket);
    }

    void stop() {

        if (serverSocket != INVALID_SOCKET) {

            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }

        WSACleanup();
    }
};