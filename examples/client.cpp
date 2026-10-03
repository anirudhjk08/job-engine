#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstring>
int main(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        std::cerr<<"WSAStartup failed\n";
        return 1;
    }
    SOCKET clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if(clientSocket == INVALID_SOCKET){
        std::cerr<<"Socket creation failed\n";
        WSACleanup();
        return 1;
    }
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );
    if(connect(
        clientSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)) == SOCKET_ERROR){
            std::cerr<<"Connection failed\n";
            closesocket(clientSocket);
            WSACleanup();

            return 1;
        }
     
        std::cout<<"Connection to server!\n";
        const char* message = "RUN_JOB";
        send(
            clientSocket,
            message,
            static_cast<int>(strlen(message)),
            0
        );
        char buffer[1024]{};

int bytesReceived = recv(
    clientSocket,
    buffer,
    sizeof(buffer) - 1,
    0
);

if (bytesReceived > 0) {
    std::cout << "Server response: " << buffer << '\n';
}
        closesocket(clientSocket);
        WSACleanup();
        return 0;
}