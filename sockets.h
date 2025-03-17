#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE 1024

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    SSL_CTX *ssl_ctx;
    SSL *ssl;
} Socket;

// Function prototypes
int create_socket(Socket *socket, const char *ip, int port);
int connect_socket(Socket *socket);
int set_socket_timeout(Socket *socket, int timeout);
int send_message(Socket *socket, const char *message);
int receive_message(Socket *socket, char *buffer, int buffer_size);
void close_socket(Socket *socket);
void initialize_ssl();
void cleanup_ssl();
void load_certificates(SSL_CTX *ctx, const char *cert_file, const char *key_file);
int validate_certificate(SSL *ssl);
void handle_errors(const char *file, int lineno, const char *msg);

#define HANDLE_ERRORS(msg) handle_errors(__FILE__, __LINE__, msg)

#endif // SOCKETS_H