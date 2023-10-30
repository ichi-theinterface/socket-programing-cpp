// server.cpp

#include <iostream>
#include <cstring>  // For strlen
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_fd, client_fd, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *message = "Hello from server";

    std::cerr << "###############" << std::endl;
    std::cerr << "# Server #" << std::endl;
    std::cerr << "###############" << std::endl;

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    std::cerr << "address.sin_port" << std::endl;
    std::cerr << address.sin_port << std::endl;
    std::cerr << address.sin_addr.s_addr << std::endl;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    } 

    // Print the server's IP and port
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ipstr, sizeof(ipstr));
    std::cout << "Server listening on " << ipstr << ":" << ntohs(address.sin_port) << std::endl;

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on " << ipstr << ":" << ntohs(address.sin_port) << std::endl;



    while (true) {  // Keep server running indefinitely
        std::cout << "Waiting for client..." << std::endl;
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;  // Continue to the next iteration of the loop
        }
    
        valread = read(client_fd, buffer, 1024);
        if (valread > 0) {
            std::cout << "Received: " << buffer << std::endl;
        } else {
            std::cerr << "Read failed or client disconnected" << std::endl;
        }
    
        send(client_fd, message, strlen(message), 0);
        std::cout << "Message sent to client" << std::endl;
    
        close(client_fd);  // Close the client socket after handling
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer for the next client
    }

// The server will never reach this point unless you add some exit mechanism
close(server_fd);
return 0;

    close(server_fd);
    return 0;
}
