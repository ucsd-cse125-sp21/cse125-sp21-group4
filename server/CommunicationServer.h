/** Communication Server
 * - Solely responsible for communicating data with the clients
 * - Pass information to the GameServer to handle physics, collisions, and objects
 */
#ifndef COMMUNICATION_SERVER_H
#define COMMUNICATION_SERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Includes the required libraries for WinSock2 Server
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#include "../common/networking/CommunicationConstants.h"
// #include "../common/PlayerPosition.h" FILE REMOVED
#include "../common/networking/GameState.h"
#include "../common/game/GameConstants.h"

struct PlayerInfo {
    int id;
    GAME_INPUT input;
    bool outputChanged;
    std::vector<char> output;
};

class CommunicationServer {
private:
    std::thread playerThreads[PLAYER_NUM]; // All the player threads
    SOCKET clientSockets[PLAYER_NUM]; // client socket fd for each player
    PlayerInfo playerInfos[PLAYER_NUM]; // Player info (like their input, output, and id)

    static void cleanUpSocket(SOCKET* clientSocketPtr);
public:
    CommunicationServer(); // Constructs the server and threads required for 4 players

    void cleanup(); // Ends the game

    void getClientInputs(std::vector<std::pair<int,GAME_INPUT>> &gameInputs); // Obtains all the inputs from the clients for processing in the GameServer
    void sendGameState(GameState gameState); // Sends the new game state to each client

    void sendGameUpdates(std::vector<GameUpdate> updates);

    static int handlePlayerThread(SOCKET* clientSocketPtr, PlayerInfo* playerInfo); // Thread for socket that will handle sending/receiving info


};

#endif