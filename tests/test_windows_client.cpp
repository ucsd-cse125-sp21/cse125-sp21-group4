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

    if(argc != 2) {
        printf("Usage: ./client.exe [ipv4 address of server]\n");
        exit(1);
    }
    CommunicationClient* commClient = new CommunicationClient();
    commClient->connectTo(std::string(argv[1]));
    /** 
     * Client is connected now. The simple client architecture goes as follows:
     *  1. Handle input (client side)
     *  2. Send input to server
     *  3. Receive updated gamestate from server
     *  4. update local game state
     *  5. Render world (client side)
     * 
     */
    GAME_INPUT sendInput;
    sendInput.input = NO_MOVE; // ' '
    // PlayerPosition playerPositions[PLAYER_NUM];

    // initialize the game instance
    Game* game = new Game();
    // game->printGameGrids();
    std::cout << "Player ID: " << commClient->getId() << "\n";
    game->printSelectingScreen();
    GameState gameState = GameState();

    sendInput.input = DONE_RENDERING;
    commClient->sendInput(sendInput);
    usleep(TICK_TIME * 1000);


    while(1) {
        
        // 1. Handle input (keyboard input here)

        // Checks if the keyboard has been hit, otherwise sendInput should be NO_MOVE
        while (_kbhit()) {
            char input = _getch();
            switch (input) {
                case 'w':
                    sendInput.input = MOVE_FORWARD;
                    break;
                case 'a':
                    sendInput.input = MOVE_LEFT;
                    break;
                case 's':
                    sendInput.input = MOVE_BACKWARD;
                    break;
                case 'd':
                    sendInput.input = MOVE_RIGHT;
                    break;
                case 'e':
                    sendInput.input = REVIVE;
                    break;
                // case 'j':
                //     sendInput.input = ATTACK;
                //     break;
                // case 'k':
                //     sendInput.input = UNIQUE_ATTACK;
                //     break;
                case '1':
                    sendInput.input = CLAIM_FIGHTER;
                    break;
                case '2':
                    sendInput.input = CLAIM_MAGE;
                    break;
                case '3':
                    sendInput.input = CLAIM_CLERIC;
                    break;
                case '4':
                    sendInput.input = CLAIM_ROGUE;
                    break;
                case 3:
                    exit(1);
            }
        } 


        // 2. Send input to server (if any)
        commClient->sendInput(sendInput);

        // 3. Receive gameState from the server
        // GameState newGameState = commClient->receiveGameState();
        std::vector<GameUpdate> updates = commClient->receiveGameUpdates();
        
        // 4. Update local game State & 5. Render the world
        // if (!game->sameGameState(gameState, newGameState)) {
        //     gameState = newGameState;
        //     game->printGameState(gameState);
        // }   
        if (updates.size() > 0) {
            std::cout << "Player ID: " << commClient->getId() << " Player Num: " << commClient->getId() + 1 << "\n";
            game->handleUpdates(updates);
            if(game->started) {
                game->printPlayers();           
            } else {
                game->printSelectingScreen();
            }
        }


        // reset send input for next input
        sendInput.input = NO_MOVE;

        usleep(5000);
        

        // do {

        //     iResult = recv(ConnectSocket, recvbuf, BUFLEN, 0);
        //     if ( iResult > 0 )
        //         printf("Bytes received: %d\n", iResult);
        //     else if ( iResult == 0 )
        //         printf("Connection closed\n");
        //     else
        //         printf("recv failed with error: %d\n", WSAGetLastError());

        // } while( iResult > 0 );
        if(!commClient->isConnected()) {
            printf("Connection lost.");
            break;
        }
    }
    delete game;
    commClient->cleanup();

    return 0;
}