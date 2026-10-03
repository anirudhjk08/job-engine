#include <iostream>
#include <thread>

#include "TcpServer.h"
#include "JobEngine.h"

void handleClient(
    TcpServer& server,
    SOCKET clientSocket,
    JobEngine& engine
) {

    std::cout << "Client connected!\n";

    std::string request =
        server.receiveMessage(clientSocket);

    std::cout << "Received: "
              << request << '\n';

    if (request == "RUN_JOB") {

        engine.submit(Job([] {
            std::cout << "Remote Job executed\n";
        }));

        server.sendResponse(
            clientSocket,
            "JOB_ACCEPTED"
        );
    }

    server.closeClient(clientSocket);
}

int main() {

    JobEngine engine(3);
    engine.start();

    TcpServer server;

    if (!server.start(8080)) {
        return 1;
    }

    while (true) {

        SOCKET clientSocket =
            server.acceptClient();

        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        std::thread(
            handleClient,
            std::ref(server),
            clientSocket,
            std::ref(engine)
        ).detach();
    }

    engine.shutdown();
    server.stop();

    return 0;
}