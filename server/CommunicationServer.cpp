#include "CommunicationServer.h"



CommunicationServer::CommunicationServer() {    
    WSADATA wsaData;

    // used to take a look at every return value from a function
    int iResult; 


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
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
    }

    // 1. Create a Listening Socket for the server to listen for any client connections
    SOCKET listeningSocket = INVALID_SOCKET;
    listeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listeningSocket == INVALID_SOCKET) {
        printf("error with socket(): %d\n", WSAGetLastError()); 
        freeaddrinfo(result); 
        WSACleanup();
    }
    
    // 2. Bind the socket to a network address
    iResult = bind(listeningSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("error with bind(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listeningSocket);
        WSACleanup();
    }

    // Bind is called, the addrinfo is no longer needed(https://docs.microsoft.com/en-us/windows/win32/winsock/binding-a-socket)
    freeaddrinfo(result);
    
    // 3. Now listen in onto any client connection requests:
    iResult = listen(listeningSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("error with listen(): %d\n", WSAGetLastError());
        closesocket(listeningSocket);
        WSACleanup();
    }

    ZeroMemory(playerInfos, sizeof(playerInfos));
    for(int i = 0; i < MAX_PLAYERS; i++) {
        playerInfos[i].id = 0;
        playerInfos[i].input = NO_MOVE;
        playerInfos[i].outputChanged = false;
    }

    // Keep on accepting connections.
    int clientID = 0;
    while (clientID < MAX_PLAYERS) {

        // 4. The accept function will block until we have received a new connection. 
        SOCKET * clientSocketPtr = (SOCKET *) (malloc (sizeof(SOCKET)));
        *clientSocketPtr = INVALID_SOCKET;

        // SOCKET WSAAPI accept(SOCKET s, sockaddr* addr, int* addrlen);
        *clientSocketPtr = accept(listeningSocket, NULL, NULL);
        if (*clientSocketPtr == INVALID_SOCKET) {
            printf("error with accept(): %d\n", WSAGetLastError());
            closesocket(listeningSocket);
            WSACleanup();
        }
        printf("ClientSocket: %d\n", *clientSocketPtr);
    
        // Gets the socket id 
        SOCKET clientSocket = *clientSocketPtr;

        // Set the client socket for this player and their id
        clientSockets[clientID] = clientSocket;
        playerInfos[clientID].id = clientID;

        // Client should probably know what player they are in the data, so send the client a byte representing their id
        iResult = send(clientSocket, (char *) &clientID, sizeof(clientID), 0);

        // Create thread for this player.
        playerThreads[clientID] = std::thread(&CommunicationServer::handlePlayerThread, clientSocketPtr, &playerInfos[clientID]);
        
        clientID++;
    }

}


// Used to clean up the socket
void CommunicationServer::cleanUpSocket(SOCKET* clientSocketPtr) {
    printf("Shutting down and cleaning socket: %d", *clientSocketPtr);

    // https://docs.microsoft.com/en-us/windows/win32/winsock/disconnecting-the-server
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(*clientSocketPtr, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("error with shutdown(): %d\n", WSAGetLastError());
        return;
    }
    closesocket(*clientSocketPtr);
    free(clientSocketPtr);
}

void CommunicationServer::cleanup() {
    WSACleanup();
}

void CommunicationServer::getClientInputs(std::vector<std::pair<int,CLIENT_INPUT>> &clientInputs) {

    // Go through each player's input and push their input to be processes if they're moves.
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(playerInfos[i].input != NO_MOVE) {

            // After getting their inputs, set it to NO_MOVE so that players can input more moves
            std::pair<int, CLIENT_INPUT> inputPair;
            inputPair.first = playerInfos[i].id;
            inputPair.second = playerInfos[i].input;
            clientInputs.push_back(inputPair);

            playerInfos[i].input = NO_MOVE;
        }
    }
}

void CommunicationServer::sendGameState(GameState gameState) {
    // serialize game state
    std::vector<char> sendbuf;
    sendbuf.assign((char*) &gameState, ((char*) &gameState) + sizeof(gameState));

    // Set the output boolean to true once the entire output is written
    for(int i = 0; i < MAX_PLAYERS; i++) {
        
        // Want to set the inputs for specific thread to send out
        std::copy(sendbuf.begin(), sendbuf.end(), back_inserter(playerInfos[i].inputs));
    }

    // Set the output boolean to true once the entire output is written so the thread can push it to the clients
    for(int i = 0; i < MAX_PLAYERS; i++) {
        playerInfos[i].outputChanged = true;
    }

}

int CommunicationServer::handlePlayerThread(SOCKET* clientSocketPtr, PlayerInfo* playerInfo) {

    SOCKET clientSocket = *clientSocketPtr;
    printf("Player Thread Launched with Socket: %d and ID: %d\n", clientSocket, playerInfo->id);

    // We want to create a player state for this client and give it one of the ID's.
    int iRecvResult, iSendResult;

    // buffer for the CLIENT_INPUT
    const int BUFLEN = sizeof(CLIENT_INPUT);
    char buf[BUFLEN];

    // set the sockets to be nonblocking
    u_long nonblocking = 1;
    int iResult = ioctlsocket(clientSocket, FIONBIO, &nonblocking);
    while(1) {
        
        // 1. Receive client input
        iRecvResult = recv(clientSocket, buf, BUFLEN, 0);
        if (iRecvResult > 0) {

            // Save the input for the GameServer to process
            playerInfo->input = *(CLIENT_INPUT *) buf;
        }

        // If the connection is closing
        else if (iRecvResult == 0) {
            printf("Connection closing...\n");
            cleanUpSocket(clientSocketPtr);
            return 1;
        }

        // There was a receive error.
        else {
            int errorCode = WSAGetLastError();
            if(errorCode == WSAEWOULDBLOCK) {
                // printf("nonblocking error from recv()\n");
                
            } else {
                printf("error with recv(): %d\n", WSAGetLastError());
                cleanUpSocket(clientSocketPtr);
                return 1;
            }
        }

        // Each player must send out the new output.
        if (playerInfo->outputChanged) {

            // 3. Send the updated state to client
            iSendResult = send(clientSocket, (char *) playerInfo->inputs.data(), playerInfo->inputs.size(), 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("error with send(): %d\n", WSAGetLastError());
                cleanUpSocket(clientSocketPtr);
                return 1;
            }
            playerInfo->outputChanged = false;
            playerInfo->inputs.clear();
        }
    }
    return 1;
}