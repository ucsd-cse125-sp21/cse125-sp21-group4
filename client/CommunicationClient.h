// From https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
// Official Microsoft Documentation (starter code)
#ifndef COMMUNICATION_CLIENT_H
#define COMMUNICATION_CLIENT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "../common/networking/CommunicationConstants.h"
#include "../common/networking/GameState.h"

class CommunicationClient {
private:
    SOCKET serverSocket;
    int id;
    GameState gameState;

public:
    CommunicationClient(); // Constructor for CommunicationClient for communication with server

    void sendInput(CLIENT_INPUT); // sendInput will send the keypress/action by client to the server
    GameState receiveGameState(); // client receives a new gameState from server and returns it to client main
    void cleanup(); // cleans up the client connection
    std::vector<GameUpdate> receiveGameUpdates();
    void validateRecv(int iResult);
};
#endif