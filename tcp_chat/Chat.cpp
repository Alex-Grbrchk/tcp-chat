#include "errors.h"
#include "Client.h"
#include "Buffer.h"
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define DEFAULT_PORT 8080
#define MESSAGE_SIZE 1024
#define SERVER_IP "127.0.0.7"
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_closed(const char* msg);

int main(int argc, char const* argv[] ){

    int client_socket;
    struct sockaddr_in server_adr;
    client_socket = Socket(AF_INET, SOCK_STREAM,0);

    server_adr.sin_port = htons(DEFAULT_PORT);
    server_adr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_adr.sin_addr);

    std::cout<<"Client socket has been created\n";

    Connect(client_socket, (struct sockaddr*) &server_adr, sizeof(server_adr));

    char message[MESSAGE_SIZE]={0};

    recv(client_socket, message, MESSAGE_SIZE, 0);

    std::cout<<"Send "<<CLIENT_CLOSE_CONNECTION_SYMBOL<<" to end connection\n"<<std::endl;
    Client New_client;
    std::cout<<"Enter your name: ";
    New_client.input_name();


    while(true){
        std::cout<<New_client.get_name() << ": ";
        Buffer buffer;
        buffer.enter_name(New_client.get_name());
        std::cin.getline(message, MESSAGE_SIZE);
        buffer.enter_message(message);
        char msg_to_send[MESSAGE_SIZE]{0};
        strcpy(msg_to_send, buffer.get_message().c_str());
        
        send(client_socket, msg_to_send, MESSAGE_SIZE, 0);
        if(is_client_connection_closed(message)){
            break;
        }
        
        recv(client_socket, message, MESSAGE_SIZE, 0);
        std::cout << message;
        if(is_client_connection_closed(message)){
            break;
        }
        std::cout<<std::endl;


    }
    close(client_socket);

}

bool is_client_connection_closed(const char* msg){
    for(int i=0; i<strlen(msg); i++){
        if(msg[i]==CLIENT_CLOSE_CONNECTION_SYMBOL){
            return true;
        }
    }
    return false;
}