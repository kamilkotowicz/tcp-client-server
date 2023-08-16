#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

const int PORT = 8888;
const int BUFFER_SIZE = 1024;
const int MAX_CONNECTIONS_QUEUE_LENGTH = 5;


void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE] = {0};

    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected\n");
            close(client_socket);
            break;
        }

        if (strcmp(buffer, "disconnect") == 0) {
            printf("Client requested disconnect\n");
            close(client_socket);
            break;
        } else if (strcmp(buffer, "hello") == 0) {
            send(client_socket, "world", strlen("world"), 0);
        } else {
            send(client_socket, buffer, bytes_received, 0);
        }

        memset(buffer, 0, sizeof(buffer));
    }

    pthread_exit(NULL);
}

void server(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    pthread_t thread;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Creating socket failed\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Bind failed\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CONNECTIONS_QUEUE_LENGTH) < 0) {
        printf("Listen failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a client...\n");

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            printf("Accept failed\n");
            exit(EXIT_FAILURE);
        }

        printf("New client connected\n");

        if (pthread_create(&thread, NULL, handle_client, (void *)&new_socket) != 0) {
            printf("Starting thread failed\n");
            close(new_socket);
        }
    }
}

void client(){
    int client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Creating socket failed\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Connect failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type 'disconnect' to exit.\n");

    while (true) {
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

        send(client_fd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "disconnect") == 0) {
            printf("Disconnecting...\n");
            break;
        }

        int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Server disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);
    }
    close(client_fd);
}

int main(int argc, char** argv){
    if(argc < 2){
        printf("specify the mode: server|client\n");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "server") == 0){
        server();
    }
    if(strcmp(argv[1], "client") == 0){
        client();
    }

    return 0;
}
