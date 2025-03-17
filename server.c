// Server side C program to demonstrate Socket programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char* hello = "Hello from server";
    ssize_t valread;

    // Read data from the client
    valread = read(new_socket, buffer, BUFFER_SIZE - 1); // subtract 1 for the null terminator at the end
    if (valread < 0) {
        perror("Read failed");
        close(new_socket);
        return;
    }

    // Null terminate the buffer
    buffer[valread] = '\0';

    // Print the received message
    printf("Received message:\n");

    // Extract and print the request line
    char* line = strtok(buffer, "\r\n");
    if (line) {
        printf("Request Line: %s\n", line);
    }

    // Extract and print headers
    printf("Headers:\n");
    while ((line = strtok(NULL, "\r\n")) && strlen(line) > 0) {
        printf("  %s\n", line);
    }

    // Send a response to the client
    if (send(new_socket, hello, strlen(hello), 0) < 0) {
        perror("Send failed");
        close(new_socket);
        return;
    }

    printf("Hello message sent\n");

    // Close the connected socket
    close(new_socket);
}

int main(int argc, char const* argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Zero out the address structure
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Handle the client in a separate function
        handle_client(new_socket);
    }

    // Close the listening socket (unreachable code, but good practice)
    close(server_fd);

    return 0;
}