// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-server-application.
// Official Microsoft Documentation 

#undef UNICODE

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

// Includes the required libraries for multithreading clients
#include <processthreadsapi.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#include "../common/constants.h"
#include "../common/PlayerPosition.h"
#include "server.h"

const int BUFLEN = sizeof(CLIENT_INPUT);
PlayerPosition playerPositions[MAX_PLAYERS];
bool playerSlots[MAX_PLAYERS];
int player_id = 0;



int main(void)
{
    WSADATA wsaData;

    // used to take a look at every return value from a function
    int iResult; 


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    // Creating a listening socket for the server. 
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    // indicates that we intend to use the socket address structure in a bind function 
    // (the bind function is to bind a local address with a socket)
    hints.ai_flags = AI_PASSIVE;

    // get the local address and port that we will use for the server
    // PADDRINFOA is just a pointer to ADDIRINFOA which is just addrinfo.
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    // If there is an error
    if (iResult != 0) {
        printf("error with getaddrinfo(): %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // 1. Create a Listening Socket for the server to listen for any client connections
    SOCKET listeningSocket = INVALID_SOCKET;
    listeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listeningSocket == INVALID_SOCKET) {
        printf("error with socket(): %d\n", WSAGetLastError()); 
        freeaddrinfo(result); 
        WSACleanup();
        return 1;
    }
    
    // 2. Bind the socket to a network address
    iResult = bind(listeningSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("error with bind(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    // Bind is called, the addrinfo is no longer needed(https://docs.microsoft.com/en-us/windows/win32/winsock/binding-a-socket)
    freeaddrinfo(result);
    
    // 3. Now listen in onto any client connection requests:
    iResult = listen(listeningSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("error with listen(): %d\n", WSAGetLastError());
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }


    // Keep on accepting connections.
    while (1) {

        // 4. The accept function will block until we have received a new connection. 
        SOCKET * clientSocketPtr = (SOCKET *) (malloc (sizeof(SOCKET)));
        *clientSocketPtr = INVALID_SOCKET;

        // SOCKET WSAAPI accept(SOCKET s, sockaddr* addr, int* addrlen);
        *clientSocketPtr = accept(listeningSocket, NULL, NULL);
        if (*clientSocketPtr == INVALID_SOCKET) {
            printf("error with accept(): %d\n", WSAGetLastError());
            closesocket(listeningSocket);
            WSACleanup();
            return 1;
        }
        printf("ClientSocket: %d\n", *clientSocketPtr);
    

        // Create thread for this player.
        CreateThread(NULL, 0, handlePlayerThread, clientSocketPtr, 0, NULL);
        
        
    }
    WSACleanup();
    return 0;
}

// Handles the player thread
DWORD WINAPI handlePlayerThread(LPVOID lpParam) { 

    // Gets the socket id 
    SOCKET * clientSocketPtr = (SOCKET *)lpParam;
    SOCKET clientSocket = *clientSocketPtr;

    // Check if we have room
    int assigned_player_id = -1;

    // Get an empty slot
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(!playerSlots[i]) {
            playerSlots[i] = true;
            assigned_player_id = i;
            break;
        }
    }

    // No more slots, clean up the socket and kick the player
    if(assigned_player_id == -1) {
        cleanUpSocket(clientSocketPtr);
        return 1;
    }
    playerPositions[assigned_player_id].id = assigned_player_id;

    printf("Player Thread Launched with Socket: %d and ID: %d\n", clientSocket, assigned_player_id);
    
    // The buffer that is received from the client
    char buf[BUFLEN + 1];
    buf[BUFLEN] = '\0';

    // We want to create a player state for this client and give it one of the ID's.

    int iRecvResult, iSendResult;

    // 5. Once accepted, the server and client can just keep talking to each other
    /**
     * Basic server architecture:
     *  1. Receive client input
     *  2. Update the game state
     *  3. Send the updated state to clients
     *  4. Wait until tick ends
     */
    while(1) {

        // 1. Receive client input
        iRecvResult = recv(clientSocket, buf, BUFLEN, 0);
        if (iRecvResult > 0) {
            printf("Player %d is was on x: %d, y: %d\n", assigned_player_id, playerPositions[assigned_player_id].x,  playerPositions[assigned_player_id].y);
            
            // 2. Update the game state
            CLIENT_INPUT input = *(CLIENT_INPUT *) buf;
            processInput(assigned_player_id, input);
        }

        
        // If the connection is closing
        else if (iRecvResult == 0) {
            printf("Connection closing...\n");
            cleanUpSocket(clientSocketPtr);
            return 1;
        }

        // There was a receive error.
        else {
            printf("error with recv(): %d\n", WSAGetLastError());
            cleanUpSocket(clientSocketPtr);
            return 1;
        }

        // Prints out to the server console the states of the clients.
        for(int i = 0; i < MAX_PLAYERS; i++) {
            printf("Player %d is now on x: %d, y: %d\n", i, playerPositions[i].x,  playerPositions[i].y); 
        }
        

        // 3. Send the updated state to client
        iSendResult = send(clientSocket, (char *) &playerPositions, sizeof(playerPositions), 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("error with send(): %d\n", WSAGetLastError());
            cleanUpSocket(clientSocketPtr);
            return 1;
        }

        // 4. Wait until tick ends
    }


    // cleanup
    cleanUpSocket(clientSocketPtr);
    return 0;
}

// Process the client's input
void processInput(int assigned_player_id, CLIENT_INPUT input) {
    switch(input) {
        case MOVE_FORWARD:
            playerPositions[assigned_player_id].y++;
            break;
        case MOVE_LEFT:
            playerPositions[assigned_player_id].x--;
            break;
        case MOVE_BACKWARD:
            playerPositions[assigned_player_id].y--;
            break;
        case MOVE_RIGHT:
            playerPositions[assigned_player_id].x++;
            break;
    }

    // Check if x and y are in bounds
    playerPositions[assigned_player_id].y = checkBounds(playerPositions[assigned_player_id].y, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
    playerPositions[assigned_player_id].x = checkBounds(playerPositions[assigned_player_id].x, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
}

// Used to clean up the socket
void cleanUpSocket(SOCKET* clientSocketPtr) {
    printf("Shutting down and cleaning socket");

    // https://docs.microsoft.com/en-us/windows/win32/winsock/disconnecting-the-server
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(*clientSocketPtr, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("error with shutdown(): %d\n", WSAGetLastError());
        cleanUpSocket(clientSocketPtr);
        return;
    }
    closesocket(*clientSocketPtr);
    free(clientSocketPtr);
}

// Check Bounds of position and if outside of bounds, set it to the bound
int checkBounds(int pos, int lower, int upper) {
    if(pos < lower) {
        return lower;
    }
    else if (pos > upper) {
        return upper;
    } 

    return pos;
}
