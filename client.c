#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9876
#define MAX_MSG_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_SIZE];

    // Create the client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    while (1) {
        // Send a message to the server
        memset(buffer, 0, MAX_MSG_SIZE);        
        printf("Client: ");
        fgets(buffer, MAX_MSG_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive a response from the server
        memset(buffer, 0, MAX_MSG_SIZE);
        recv(client_socket, buffer, MAX_MSG_SIZE, 0);
        printf("Server: %s\n", buffer);

        if (strcasecmp("Exit", buffer) == 0)
            break;
    }

    close(client_socket);
    return 0;
}
