#include <iostream>
#include <cstring>  // For strlen
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>    // For fcntl
#include <sys/select.h> // For select
#include <errno.h>    // For errno and error codes


int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    const char *message = "Hello from client";
    char buffer[1024] = {0};
    std::string ipAddress;  // This will store the user-entered IP address

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Ask user for IP address
    std::cout << "Please enter the IP address of the server: " << std::endl;
    std::cout << "e.g. 127.0.0.1 for local host" << std::endl;
    std::cin >> ipAddress;

    if (inet_pton(AF_INET, ipAddress.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported" << std::endl;
        return -1;
    }

    // Set the socket to non-blocking mode
    fcntl(sock, F_SETFL, O_NONBLOCK);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if (errno != EINPROGRESS) {
            std::cerr << "Connection attempt failed" << std::endl;
            close(sock);
            return -1;
        }
    }

    fd_set fdset;
    struct timeval timeout;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    timeout.tv_sec = 3;  // 3 seconds timeout
    timeout.tv_usec = 0;

    if (select(sock + 1, NULL, &fdset, NULL, &timeout) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error == 0) {
            // Connection successful
        } else {
            std::cerr << "Connection failed after timeout" << std::endl;
            close(sock);
            return -1;
        }
    } else {
        std::cerr << "Connection timed out" << std::endl;
        close(sock);
        return -1;
    }

    send(sock, message, strlen(message), 0);
    std::cout << "Message sent to server" << std::endl;

    valread = read(sock, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;

    close(sock);
    return 0;
}
