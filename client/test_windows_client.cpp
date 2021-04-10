// From https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
// Official Microsoft Documentation (starter code)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// Conio used for keypresses
#include <conio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "../common/constants.h"
#include "../common/PlayerPosition.h"

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    int iResult;
    


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    SOCKET ConnectSocket = INVALID_SOCKET;
    iResult = getaddrinfo(ERICS_LOCAL_SERVER, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    int playerID = -1;
    iResult = recv(ConnectSocket, (char*) &playerID, sizeof(playerID), 0);
    if ( iResult > 0 ) {
        printf("Bytes received: %d\n", iResult);
        printf("Player ID: %d", playerID);
    }
        

    /** 
     * Client is connected now. The simple client architecture goes as follows:
     *  1. Handle input (client side)
     *  2. Send input to server
     *  3. Receive updated gamestate from server
     *  4. update local game state
     *  5. Render world (client side)
     * 
     */
    CLIENT_INPUT sendInput;
    PlayerPosition playerPositions[MAX_PLAYERS];
    while(1) {
        
        // 1. Handle input (keyboard input here)

        // Checks if the keyboard has been hit, otherwise sendInput should be NO_MOVE
        if (_kbhit()) {
            char input = _getch();
            switch (input) {
                case 'w':
                    sendInput = MOVE_FORWARD;
                    break;
                case 'a':
                    sendInput = MOVE_LEFT;
                    break;
                case 's':
                    sendInput = MOVE_BACKWARD;
                    break;
                case 'd':
                    sendInput = MOVE_RIGHT;
                    break;
                case 3:
                    exit(1);
            }
        } else {
            sendInput = NO_MOVE;
        }
        usleep(25000);


        // 2. Send input to server (if any)
        iResult = send( ConnectSocket, (char *)&sendInput, sizeof(CLIENT_INPUT), 0 );
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }


        // 3. Receive until the peer closes the connection
        iResult = recv(ConnectSocket, (char *) &playerPositions, sizeof(playerPositions), 0);
        if ( iResult > 0 ) {
            printf("Bytes received: %d\n", iResult);
            printf("Player %d is on x: %d, y: %d\n", playerPositions[0].id, playerPositions[0].x, playerPositions[0].y);
            printf("Player %d is on x: %d, y: %d\n", playerPositions[1].id, playerPositions[1].x, playerPositions[1].y);
            printf("Player %d is on x: %d, y: %d\n", playerPositions[2].id, playerPositions[2].x, playerPositions[2].y);
            printf("Player %d is on x: %d, y: %d\n", playerPositions[3].id, playerPositions[3].x, playerPositions[3].y);
        }
        else if ( iResult == 0 ) {
            printf("Connection closed\n");
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
        }
        

        // do {

        //     iResult = recv(ConnectSocket, recvbuf, BUFLEN, 0);
        //     if ( iResult > 0 )
        //         printf("Bytes received: %d\n", iResult);
        //     else if ( iResult == 0 )
        //         printf("Connection closed\n");
        //     else
        //         printf("recv failed with error: %d\n", WSAGetLastError());

        // } while( iResult > 0 );

    }


    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}