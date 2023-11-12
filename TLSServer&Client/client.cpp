#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // for Mac

#define PORT 420

int main() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    connect(client_fd, (struct sockaddr *)&address, sizeof(address));

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);
    SSL_connect(ssl);

    SSL_write(ssl, "Hello, server!", 15);

    SSL_free(ssl);
    close(client_fd);
    SSL_CTX_free(ctx);
    return 0;
}
