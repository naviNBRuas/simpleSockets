#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sockets.h"

#define PORT 8080

int main(int argc, char const *argv[]) {
    Socket socket;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // Create socket
    if (create_socket(&socket, "127.0.0.1", PORT) < 0) {
        return EXIT_FAILURE;
    }

    // Connect to server
    if (connect_socket(&socket) < 0) {
        return EXIT_FAILURE;
    }

    // Set socket timeout
    set_socket_timeout(&socket, 5);

    printf("Connected to server at 127.0.0.1:%d\n", PORT);

    while (1) {
        // Get user input
        printf("Enter message to send (or 'exit' to quit): ");
        if (!fgets(message, BUFFER_SIZE, stdin)) {
            printf("Error reading input.\n");
            break;
        }
        message[strcspn(message, "\n")] = 0; // Remove newline character

        // Check for exit condition
        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Send message to server
        if (send_message(&socket, message) < 0) {
            return EXIT_FAILURE;
        }

        // Read server response
        if (receive_message(&socket, buffer, BUFFER_SIZE) < 0) {
            return EXIT_FAILURE;
        }

        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close_socket(&socket);

    return EXIT_SUCCESS;
}
