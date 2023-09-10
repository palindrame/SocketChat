#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9876
#define MAX_MSG_SIZE 1024

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[MAX_MSG_SIZE];

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket to the specified address and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    // Listen for incoming connections
    listen(server_socket, 10);
    printf("Server listening on port %d...\n", PORT);

    addr_size = sizeof(new_addr);
    new_socket = accept(server_socket, (struct sockaddr*)&new_addr, &addr_size); // Accept a connection
    if (new_socket < 0) {
        perror("Error in accepting");
        exit(1);
    }

    while (1) {
        // Receive a message from the client
        memset(buffer, 0, MAX_MSG_SIZE);        
        recv(new_socket, buffer, MAX_MSG_SIZE, 0);
        printf("Client: %s\n", buffer);

        // Send a response to the client
        printf("Server: ");
        memset(buffer, 0, MAX_MSG_SIZE);
        fgets(buffer, MAX_MSG_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);

        if(strcasecmp("Exit", buffer) == 0)
            break;
    }

    close(new_socket);
    close(server_socket);
    return 0;
}

