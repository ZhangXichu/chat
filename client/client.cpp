/**
 * @brief legacy client
 */

#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h> // for close
#include <constants.hpp>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./client [ip address] [port number]" << std::endl;
        return 1;
    }

    // get ip address and port number
    char host_address[256];
    std::strncpy(host_address, argv[1], sizeof(host_address) - 1);
    host_address[sizeof(host_address) - 1] = '\0';
    int port = std::atoi(argv[2]);

    struct sockaddr_in client_addr{};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host_address, &client_addr.sin_addr.s_addr) <= 0)
    {
        std::cerr << "Invalid ip address." << std::endl;
    }

    // create the socket 
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("Socket creation failed.");
        return 1;
    }

    // issue connection request to server
    int connect_status = connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (connect_status == -1)
    {
        perror("Failed to connect to the server");
        return 1;
    }
    std::cout << "Connected to the server."  << std::endl;

    char buffer[MAX_MSG_LEN];
    while (true)
    {       
        std::cout << ">";
        std::cin.getline(buffer, sizeof(buffer));

        size_t len = std::strlen(buffer);
        
        buffer[len] = '\n';      
        buffer[len + 1] = '\0'; 

        ssize_t bytes_sent = send(client_fd, buffer, std::strlen(buffer), 0);
        if (bytes_sent == -1)
        {
            perror("Failed to send message to the server.");
            close(client_fd);
            return 1;
        }

        std::cout << "Msg sent: " << buffer << std::endl;
    }

    close(client_fd);

    return 0;
}