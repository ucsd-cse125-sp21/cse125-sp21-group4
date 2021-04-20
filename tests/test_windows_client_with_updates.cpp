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

#include "../common/networking/CommunicationConstants.h"
#include "../client/CommunicationClient.h"

#include "../common/game/Game.h"

int __cdecl main(int argc, char **argv) 
{
    CommunicationClient* commClient = new CommunicationClient();
    /** 
     * Client is connected now. The simple client architecture goes as follows:
     *  1. Handle input (client side)
     *  2. Send input to server
     *  3. Receive updated gamestate from server
     *  4. update local game state
     *  5. Render world (client side)
     * 
     */
    CLIENT_INPUT sendInput = NO_MOVE; // ' '
    // PlayerPosition playerPositions[MAX_PLAYERS];

    // initialize the game instance
    Game* game = new Game();
    // game->printGameGrids();
    game->printPlayers();

    while(1) {
        
        // 1. Handle input (keyboard input here)

        // Checks if the keyboard has been hit, otherwise sendInput should be NO_MOVE
        while (_kbhit()) {
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
                case 'j':
                    sendInput = ATTACK;
                    break;
                case 3:
                    exit(1);
            }
        } 


        // 2. Send input to server (if any)
        commClient->sendInput(sendInput);

        // 3. Receive gameState from the server
        // GameState gameState = commClient->receiveGameState();
        std::vector<GameUpdate> updates = commClient->receiveGameUpdates();

        for(int i = 0; i < updates.size(); i++) {
            GameUpdate update = updates[i];
            printf("ID: %d, gridX: %d, gridY: %d, floatDeltaX: %f, floatDeltaY: %f\n", update.id, update.gridPos.x, update.gridPos.y, update.floatDeltaX, update.floatDeltaY);
        }

        
        // 4. Update local game State
        // std::cout << "gameState inputs: " << gameState.playersInputs.size() << std::endl;
        // bool isChanged = game->handleInputs(gameState.playersInputs);

        // 5. Render the world
        // if (isChanged) {
        //     game->printGameGrids();
        //     game->printPlayers();
        // }


        // reset send input for next input
        sendInput = NO_MOVE;

        usleep(25000);
        

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
    delete game;
    commClient->cleanup();

    return 0;
}