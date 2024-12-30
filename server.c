#include <stdio.h>
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>    // close
#include <string.h> //strlen

#define PORT 3490
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main() {

    // AF_INET - IPv4
    // SOCK_STREAM - TCP protocol
    // 0 - choose defualt protocol

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0 ) {
        perror("failed to create socket");
        return 1;

    }

    // Set socket option to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Failed to set socket option");
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(PORT); // port number 

    //bind the socket - casting address struct to sockaddr *
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("failed to bind");
        return 1;
    }
    
    //listen for connections
    if (listen(server_fd, BACKLOG) < 0 ) {
        perror("failed to find connections");
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // accept a connection 
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (new_socket < 0) {
        perror("failed to accept connection");
        return 1;
    }

    printf("Connection accepted \n");

    char buffer[BUFFER_SIZE] = {0};
    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0 ) {
            perror("Client Disconnected\n");
            break;
        }   

        printf("Received: %s", buffer);

        //exit check
        if(strcmp(buffer, "exit\n") == 0) {
            printf("Exit command received. Closing server...\n");
            break;
        }

        //echo received message
        send(new_socket, buffer, strlen(buffer), 0);

    }


    close(new_socket);
    close(server_fd);

    return 0;

}
