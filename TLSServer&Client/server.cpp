#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  // for Mac

#define PORT 420

int main() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);

    if (!SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) ||
        !SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM)) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    int client_fd = accept(server_fd, NULL, NULL);
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);
    SSL_accept(ssl);

    char buf[1024];
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = '\0';
    std::cout << "Received: " << buf << std::endl;

    SSL_free(ssl);
    close(client_fd);
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}
