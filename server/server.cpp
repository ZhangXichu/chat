/**
 * @brief for legacy client
 */
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <netinet/in.h> // for internet domain addresses
#include <unistd.h> // for close
#include <sys/types.h>
#include <sys/socket.h>
#include <constants.hpp>

std::mutex clnt_mtx;

std::vector<int> clnt_fds;

void handle_client(int clnt_fd)
{
    char buffer[MAX_MSG_LEN + 1];
    std::string message_buffer;

    while (true)
    {
        ssize_t bytes_received = recv(clnt_fd, buffer, sizeof(buffer), 0);
        std::cout << "bytes_received: " << bytes_received << std::endl;
        if (bytes_received <= 0)
        {
            break;
        } 
        
        buffer[bytes_received] = '\0';
        message_buffer += buffer;

        size_t pos;
        while ((pos = message_buffer.find('\n')) != std::string::npos)
        {
            // Extract message
            std::string message = message_buffer.substr(0, pos); 
            std::cout << "client" << clnt_fd << " says : " << message << std::endl;
            message_buffer.erase(0, pos + 1);
        }

    }
    close(clnt_fd);
    std::cout << "Connection with client " << clnt_fd << " closed." << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./server [port number]" << std::endl;
        return 1;
    }

    // get port number
    int port = std::atoi(argv[1]);
    
    struct sockaddr_in server_addr{};
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
        std::cout << "new iter" << std::endl;
        struct sockaddr_in client_addr;
        socklen_t clnt_addr_size = sizeof(client_addr);

        // create new socket file sescriptor
        int clnt_fd = accept(server_fd, (struct sockaddr*)&server_addr, &clnt_addr_size);
        if (clnt_fd < 0)
        {
            perror("Accept failed");
            continue;
        } else {
            std::cout << "New client " << clnt_fd << " connected." << std::endl;
            {
                std::lock_guard<std::mutex> lock(clnt_mtx);
                clnt_fds.push_back(clnt_fd);
            }
        }
        std::thread client_thread(handle_client, clnt_fd);
        client_thread.detach();
    }

    close(server_fd);

    return 0;

}