#include "sockets.h"

void handle_errors(const char *file, int lineno, const char *msg) {
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
}

void initialize_ssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

void cleanup_ssl() {
    EVP_cleanup();
}

void load_certificates(SSL_CTX *ctx, const char *cert_file, const char *key_file) {
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        HANDLE_ERRORS("Unable to load certificate");
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        HANDLE_ERRORS("Unable to load private key");
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        HANDLE_ERRORS("Private key does not match the certificate public key");
    }
}

int create_socket(Socket *socket, const char *ip, int port) {
    socket->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket->sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    socket->address.sin_family = AF_INET;
    socket->address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &socket->address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }
    return 0;
}

int connect_socket(Socket *socket) {
    if (connect(socket->sockfd, (struct sockaddr*)&socket->address, sizeof(socket->address)) < 0) {
        perror("Connection failed");
        return -1;
    }
    return 0;
}

int set_socket_timeout(Socket *socket, int timeout) {
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    if (setsockopt(socket->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) < 0) {
        perror("Setting socket timeout failed");
        return -1;
    }
    return 0;
}

int send_message(Socket *socket, const char *message) {
    int len = SSL_write(socket->ssl, message, strlen(message));
    if (len < 0) {
        perror("Send message failed");
        return -1;
    }
    return len;
}

int receive_message(Socket *socket, char *buffer, int buffer_size) {
    int len = SSL_read(socket->ssl, buffer, buffer_size);
    if (len < 0) {
        perror("Receive message failed");
        return -1;
    }
    buffer[len] = '\0';
    return len;
}

void close_socket(Socket *socket) {
    if (socket->ssl) {
        SSL_shutdown(socket->ssl);
        SSL_free(socket->ssl);
    }
    close(socket->sockfd);
    if (socket->ssl_ctx) {
        SSL_CTX_free(socket->ssl_ctx);
    }
}