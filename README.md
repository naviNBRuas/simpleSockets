# Secure Socket Communication Library

This project provides a set of C functions for creating, connecting, sending, and receiving messages over a secure socket connection using SSL/TLS.

## Usage

The project includes the following functions:

* **`initialize_ssl()`:** Initializes OpenSSL library and loads error strings.
* **`cleanup_ssl()`:** Cleans up OpenSSL resources.
* **`load_certificates(SSL_CTX *ctx, const char *cert_file, const char *key_file)`:** Loads the certificate and private key from PEM files.
* **`create_socket(Socket *socket, const char *ip, int port)`:** Creates a socket and binds it to the specified address and port.
* **`connect_socket(Socket *socket)`:** Connects the socket to the remote server.
* **`set_socket_timeout(Socket *socket, int timeout)`:** Sets the socket receive timeout.
* **`send_message(Socket *socket, const char *message)`:** Sends a message over the SSL connection.
* **`receive_message(Socket *socket, char *buffer, int buffer_size)`:** Receives a message from the SSL connection.
* **`close_socket(Socket *socket)`:** Closes the socket and frees SSL resources.

## Example

The `client.c` and `server.c` files provide example implementations for a client and server application using the library.

**Client Example:**

```c
#include "sockets.h"
#include <stdio.h>

int main() {
    Socket socket;
    char buffer[BUFFER_SIZE];

    // Initialize SSL
    initialize_ssl();

    // Create socket
    if (create_socket(&socket, "127.0.0.1", 8080) < 0) {
        return EXIT_FAILURE;
    }

    // Connect to server
    if (connect_socket(&socket) < 0) {
        return EXIT_FAILURE;
    }

    // Send message
    if (send_message(&socket, "Hello from client!") < 0) {
        return EXIT_FAILURE;
    }

    // Receive message
    if (receive_message(&socket, buffer, BUFFER_SIZE) < 0) {
        return EXIT_FAILURE;
    }

    printf("Server response: %s\n", buffer);

    // Close socket
    close_socket(&socket);

    // Cleanup SSL
    cleanup_ssl();

    return EXIT_SUCCESS;
}
```
**Server Example:**

```c
#include "sockets.h"
#include <stdio.h>
#include <unistd.h>
int main() {
    Socket socket;
    char buffer[BUFFER_SIZE];
    // Initialize SSL
    initialize_ssl();
    // Load certificates
    load_certificates(socket.ssl_ctx, "server.crt", "server.key");
    // Create socket
    if (create_socket(&socket, "0.0.0.0", 8080) < 0) {
        return EXIT_FAILURE;
    }
    // Listen for incoming connections
    listen(socket.sockfd, 5);
    // Accept incoming connection
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_fd = accept(socket.sockfd, (struct sockaddr*)&client_address, &client_address_len);
    // Receive message
    if (receive_message(&socket, buffer, BUFFER_SIZE) < 0) {
        return EXIT_FAILURE;
    }
    printf("Client message: %s\n", buffer);
    // Send message
    if (send_message(&socket, "Hello from server!") < 0) {
        return EXIT_FAILURE;
    }
    // Close socket
    close_socket(&socket);
    // Cleanup SSL
    cleanup_ssl();
    return EXIT_SUCCESS;
}
```
## Requirements
OpenSSL library (install using apt-get install libssl-dev or equivalent)
GCC compiler