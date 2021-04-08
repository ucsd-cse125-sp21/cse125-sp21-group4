#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#define SERVER_PORT 27015

#include "PlayerPosition.h"
typedef struct PlayerPosition PlayerPosition;

const int BUFLEN = sizeof(PlayerPosition);

int main(int argc, char * argv[]) {
    

    int result, server_socket;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        fprintf(stderr, "error in socket(): %d", errno);
        fprintf(stderr, "error in socket(): %s", strerror(errno));
        return 1;
    }

    // Use gethostbyname --> to get hostent of server
    struct hostent * server_entity = gethostbyname(SERVER_IP);
    if(server_entity == NULL) {
        fprintf(stderr, "error in gethostbyname(): %d", errno);
        fprintf(stderr, "error in gethostbyname(): %s", strerror(errno));
        return 1;
    }

    // Get the server addr
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // server.h_addr is a char *, want to just copy it over to server_addr.
    bcopy(server_entity->h_addr, (char *) &server_addr.sin_addr.s_addr, server_entity->h_length);


    // Connect to server
    result = connect(server_socket, &server_addr, sizeof(server_addr));
    if(result == -1) {
        fprintf(stderr, "error in connect(): %d", errno);
        fprintf(stderr, "error in connect(): %s", strerror(errno));
        return 1;
    }

    PlayerPosition * player = malloc(sizeof(PlayerPosition));
    player->id = 1;
    player->x = 0;
    player->y = 0;
    PlayerPosition * player2 = malloc(sizeof(PlayerPosition));

    printf("Connected to server.");
    while(1) {
        char c = getchar();
        switch(c) {
            case 'w':
                player->y++;
                break;
            case 'a':
                player->x--;
                break;
            case 's':
                player->y--;
                break;
            case 'd':
                player->x++;
                break;
        }
        printf("After keypress: Player %d is on x: %d, y: %d", player->id, player->x, player->y);

        // Send a word to server
        result = write(server_socket, player, BUFLEN);

        // Read a word from server
        result = read(server_socket, player2, BUFLEN);

        printf("Returned: Player %d is on x: %d, y: %d", player2->id, player2->x, player2->y);

    }


    // Disconnect from server
    printf("Closing client socket...");
    close(server_socket);

    return 0;
}