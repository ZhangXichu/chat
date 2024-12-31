#include <iostream>
#include <netinet/in.h> // for internet domain addresses
#include <unistd.h> // for close
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_CLNT 10

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./server [port number]" << std::endl;
    }

    // get port number
    int port = std::atoi(argv[1]);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // bind to any address
    server_addr.sin_port = htons(port);

    // create the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // use default protocol for stream, which is tcp
    if (server_fd == -1)
    {
        perror("Failed to create socket.");
    }

    // bind local ip address
    struct sockaddr addr;
    int bind_status = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_status == -1)
    {
        perror("Failed to bind address to socket.");
        close(server_fd);
        return 1;
    }

    // start listening for connections
    int listen_status = listen(server_fd, MAX_CLNT);
    if (listen_status == -1)
    {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }
    std::cout << "Server is listening on port " << port << std::endl;

    // accepting requests
    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t clnt_addr_size = sizeof(client_addr);

        // create new nonblocking socket file sescriptor
        int clnt_fd = accept4(server_fd, (struct sockaddr*)&server_addr, &clnt_addr_size, SOCK_NONBLOCK);
        if (clnt_fd < 0)
        {
            perror("Accept failed");
            return 1;
        }
    }

}