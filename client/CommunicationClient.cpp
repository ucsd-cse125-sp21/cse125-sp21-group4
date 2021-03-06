#include "CommunicationClient.h"


CommunicationClient::CommunicationClient() {
    connected = false;
}

bool CommunicationClient::connectTo(std::string serverIP) {

    WSADATA wsaData;
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    // setup the address info for the server to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    serverSocket = INVALID_SOCKET;
    iResult = getaddrinfo(serverIP.c_str(), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        serverSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (serverSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( serverSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (serverSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
    
    id = -1;
    iResult = recv(serverSocket, (char*) &id, sizeof(id), 0);
    if ( iResult > 0 ) {
        // printf("Bytes received: %d\n", iResult);
        // printf("Player ID: %d", id);
    }

    // Make the server socket non-blocking
    u_long nonblocking = 1;
    ioctlsocket(serverSocket, FIONBIO, &nonblocking);

    connected = true;
    return true;
}

void CommunicationClient::sendInput(GAME_INPUT input) {
    int iResult;
    // 2. Send input to server (if any)
    if(input.input != NO_MOVE) {
        iResult = send( serverSocket, (char *)&input, sizeof(GAME_INPUT), 0 );
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            connected = false;
            // exit(1);
        }
    }
}
GameState CommunicationClient::receiveGameState() {

    // 3. Receive the game state
    int iResult = recv(serverSocket, (char *) &gameState, sizeof(gameState), 0);
    validateRecv(iResult);

    return gameState;
}

void CommunicationClient::cleanup() {
    
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(serverSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        // exit(1);
    }

    // cleanup
    connected = false;
    closesocket(serverSocket);
    WSACleanup();
}


std::vector<GameUpdate> CommunicationClient::receiveGameUpdates() {
    std::vector<GameUpdate> updates;

    // Get the amount of bytes of updates we should get
    int numUpdates = 0;
    int iResult = recv(serverSocket, (char *) &numUpdates, sizeof(int), 0);
    validateRecv(iResult);
    // printf("Number of Updates: %d\n", numUpdates);

    if(numUpdates == 0) {
        return updates;
    }
    // printf("Number of Actual Updates: %d\n", numUpdates);

    // Now receive that many bytes of input
    updates.resize(numUpdates);
    iResult = recv(serverSocket, (char *) &updates[0], numUpdates * sizeof(GameUpdate), 0);
    validateRecv(iResult);

    return updates;
}


void CommunicationClient::validateRecv(int iResult) {
    if ( iResult > 0 ) {
        // printf("Bytes received: %d\n", iResult);
    }
    // Disconnection
    else if ( iResult == 0 ) {
        printf("Connection closed\n");
        closesocket(serverSocket);
        WSACleanup();
        // exit(1);
        connected = false;

    }

    // Errors with recv
    else {
        int errorCode = WSAGetLastError();
        if(errorCode == WSAEWOULDBLOCK) {
            // printf("nonblocking error from recv()\n");
            
        } else {
            printf("error with recv(): %d\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup(); 
            connected = false;

            // exit(1);
        }
    }

}

int CommunicationClient::getId() {
    return id;
}

bool CommunicationClient::isConnected() {
    return connected;
}