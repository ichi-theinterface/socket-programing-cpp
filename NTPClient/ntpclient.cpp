#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull
#define NTP_PORT 123

struct ntp_packet {
    uint8_t li_vn_mode;      // Leap indicator, version number, and mode
    uint8_t stratum;         // Stratum level
    uint8_t poll;            // Poll interval
    uint8_t precision;       // Precision
    uint32_t root_delay;     // Root delay
    uint32_t root_dispersion; // Root dispersion
    uint32_t ref_id;         // Reference ID
    uint32_t ref_timestamp_secs; // Reference timestamp (seconds)
    uint32_t ref_timestamp_frac; // Reference timestamp (fractions)
    uint32_t orig_timestamp_secs; // Origin timestamp (seconds)
    uint32_t orig_timestamp_frac; // Origin timestamp (fractions)
    uint32_t rx_timestamp_secs; // Received timestamp (seconds)
    uint32_t rx_timestamp_frac; // Received timestamp (fractions)
    uint32_t tx_timestamp_secs; // Transmit timestamp (seconds)
    uint32_t tx_timestamp_frac; // Transmit timestamp (fractions)
};

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create a UDP socket
    if (sockfd < 0) {
        std::cerr << "Error: Unable to create socket." << std::endl;
        return -1;
    }

    // Define the server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NTP_PORT);

    // Resolve the server address (time.google.com)
    struct hostent *server = gethostbyname("time.google.com");
    if (server == nullptr) {
        std::cerr << "Error: No such host." << std::endl;
        return -1;
    }
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Initialize and set up the packet structure
    ntp_packet packet = {};
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = 0x1b; // Set the leap indicator, version and mode

    // Send the packet
    if (sendto(sockfd, (char*)&packet, sizeof(ntp_packet), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error: Failed to send packet." << std::endl;
        close(sockfd);
        return -1;
    }

    // Receive the packet
    unsigned int serv_addr_len = sizeof(serv_addr);
    if (recvfrom(sockfd, (char*)&packet, sizeof(ntp_packet), 0, (struct sockaddr*)&serv_addr, &serv_addr_len) < 0) {
        std::cerr << "Error: Failed to receive packet." << std::endl;
        close(sockfd);
        return -1;
    }

    // Convert timestamps from network byte order to host byte order
    packet.tx_timestamp_secs = ntohl(packet.tx_timestamp_secs);
    packet.tx_timestamp_frac = ntohl(packet.tx_timestamp_frac);

    // Calculate the time - convert it to Unix time format
    time_t tx_time = (packet.tx_timestamp_secs - NTP_TIMESTAMP_DELTA);

    std::cout << "Time received from NTP server: " << ctime(&tx_time);

    close(sockfd);
    return 0;
}
