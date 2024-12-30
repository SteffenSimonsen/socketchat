#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // for inet_addr
#include <unistd.h>     // for close()
#include <string.h> //strlen

#define PORT 3490
#define BUFFER_SIZE 1024

int main() {

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("failed to bind\n");
        return 1;
    }

    //server addr struct
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect to server 
    if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed\n");
        return 1;
    }

    printf("Connected to server! To quit type 'exit'.\n");
    printf("Type your messages: \n");

    char buffer[BUFFER_SIZE];

    while (1) {
        printf(">");

        //get input from user

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL){
            break;
        }

        send(client_fd, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_read <= 0 ){
            printf("Server disconnected \n");
            break;
        } 

        printf("Server echo: %s", buffer);

    }
    
    close(client_fd);

}
