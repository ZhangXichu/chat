#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

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

    const char *message = "First message";
    while (true)
    {       

    }

    return 0;
}