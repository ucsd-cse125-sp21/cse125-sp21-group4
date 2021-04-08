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

#define DEFAULT_PORT "27015"
#define MAX_PLAYERS 4

#include "PlayerPosition.h"

const int BUFLEN = sizeof(PlayerPosition);

// Handles the player thread
DWORD WINAPI handlePlayerThread(LPVOID lpParam) { 

    // Gets the socket id 
    SOCKET clientSocket = *(SOCKET *)lpParam;

    printf("ClientSocket1: %d\n", clientSocket);
    
    // The buffer that is received from the client and sent back (Echo)
    char buf[BUFLEN + 1];
    buf[BUFLEN] = '\0';
    int iRecvResult, iSendResult;




    // 5. Once accepted, the server and client can just keep talking to each other
    do {

        // Accepts data from sender
        iRecvResult = recv(clientSocket, buf, BUFLEN, 0);
        if (iRecvResult > 0) {
            // printf("%s\n", buf);
            PlayerPosition* player = (PlayerPosition *)buf;
            printf("Player %d is on x: %d, y: %d\n", player->id, player->x, player->y);

            // Sends to the client the data back
            iSendResult = send(clientSocket, buf, BUFLEN, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("error with send(): %d\n", WSAGetLastError());
                closesocket(clientSocket);
                return 1;
            }
        }
        
        // If the connection is closing
        else if (iRecvResult == 0) {
            printf("Connection closing...\n");
        }

        // There was a recieve error.
        else {
            printf("error with recv(): %d\n", WSAGetLastError());
            closesocket(clientSocket);
            return 1;
        }
    } while (iRecvResult > 0);

    // https://docs.microsoft.com/en-us/windows/win32/winsock/disconnecting-the-server
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("error with shutdown(): %d\n", WSAGetLastError());
        closesocket(clientSocket);
        return 1;
    }

    // cleanup
    closesocket(clientSocket);
    return 0;
}


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
        printf("ClientSocket2: %d\n", *clientSocketPtr);
    

        // Create thread for this player.
        CreateThread(NULL, 0, handlePlayerThread, clientSocketPtr, 0, NULL);
        
        
    }
    WSACleanup();
    return 0;
}
