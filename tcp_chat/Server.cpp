#include "errors.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <thread>
#include <vector>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_closed(const char* msg);

void handle_client(int client_socket, std::vector<int>& client_sockets);

int main(int argc, char const* argv[] ){

    int server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_adr{0};
    server_adr.sin_family = AF_INET;
    server_adr.sin_port = htons(DEFAULT_PORT);
    server_adr.sin_addr.s_addr = htons(INADDR_ANY);
    socklen_t adr_len = sizeof(server_adr);

    Bind(server_fd, (struct sockaddr *) &server_adr, sizeof(server_adr));
    
    Listen(server_fd, 4);

    std::vector<int> client_sockets;

    while (true) {
        std::cout << "Waiting for connections..." << std::endl;
        int client_socket = Accept(server_fd, (struct sockaddr *) &server_adr, &adr_len);
        std::cout << "New connection accepted" << std::endl;
        
        client_sockets.push_back(client_socket);
        std::thread client_thread(handle_client, client_socket, std::ref(client_sockets));
        client_thread.detach();
    }

    
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}

void handle_client(int client_socket, std::vector<int>& client_sockets) {
    char buffer[BUFFER_SIZE] = {0};
    bool is_exit = false;
    strcpy(buffer, "Server is connected");
    send(client_socket, buffer, strlen(buffer), 0);
    while (!is_exit) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            std::cerr << "Error receiving data from client" << std::endl;
            break;
        }
        else if (bytes_received == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        else {
            std::cout << "Received message from client: " << buffer << std::endl;

            if (is_client_connection_closed(buffer)) {
                is_exit = true;
                break;
            }

            // Send the message to all other clients
            for (int i = 0; i < client_sockets.size(); i++) {
                if (client_sockets[i] != client_socket) {
                    int bytes_sent = send(client_sockets[i], buffer, bytes_received, 0);
                    if (bytes_sent < 0) {
                        std::cerr << "Error sending data to client" << std::endl;
                        break;
                    }
                }
            }
        }
    }
    
    // Remove the client socket from the vector
    /*auto it = std::find(std::begin(client_sockets), std::end(client_sockets), client_socket);
    if (it != client_sockets.end()) {
        client_sockets.erase(it);
    }*/

    for(int i = 0; i < client_sockets.size(); i++) {
        if(client_sockets[i] == client_socket){
        client_sockets.erase(client_sockets.begin()+i);
        break;
    }   
    
}

    close(client_socket);
}

bool is_client_connection_closed(const char* msg) {
    // Check if the message contains the client close connection symbol
    for (int i = 0; i < strlen(msg); i++) {
        if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }   
    return false;
}
