#include "errors.h"
#include "Client.h"
#include "Buffer.h"
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <thread>
#include <sys/select.h> // для функции select

#define DEFAULT_PORT 8080
#define MESSAGE_SIZE 1024
#define SERVER_IP "127.0.0.7"
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_closed(const char* msg);

void receive_messages(int client_socket);

void send_messages(int client_socket, Client& New_client);

int main(int argc, char const* argv[]) {

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

    std::thread receiver_thread(receive_messages, client_socket);
    std::thread sender_thread(send_messages, client_socket, std::ref(New_client));

    receiver_thread.join();
    sender_thread.join();

    close(client_socket);

    return 0;
}

bool is_client_connection_closed(const char* msg) {
    for (int i=0; i<strlen(msg); i++) {
        if (msg[i]==CLIENT_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}

void send_messages(int client_socket, Client& New_client) {
    char message[MESSAGE_SIZE]={0};
    fd_set readSet;
    struct timeval tv; // структура для задания таймаута select

    while (true) {
        FD_ZERO(&readSet); // очистка набора файловых дескрипторов
        FD_SET(STDIN_FILENO, &readSet); // добавление стандартного ввода в набор

        tv.tv_sec = 1; // задание таймаута select на 1 секунду
        tv.tv_usec = 0;

        int ready = select(STDIN_FILENO + 1, &readSet, NULL, NULL, &tv); // проверка доступности данных для чтения

        if (ready == -1) {
            // ошибка вызова select
            std::cerr << "Error in select" << std::endl;
            break;
        } 
        else if (ready == 0) {
            
        } 
        else {
            
            std::cin.getline(message, MESSAGE_SIZE);
            if (is_client_connection_closed(message)) {
                break;
            }
            Buffer buffer;
            buffer.enter_name(New_client.get_name());
            buffer.enter_message(message);
            char msg_to_send[MESSAGE_SIZE]={0};
            strcpy(msg_to_send, buffer.get_message().c_str());
            send(client_socket, msg_to_send, MESSAGE_SIZE, 0);
        
        }
        
        
        
    }
}

void receive_messages(int client_socket) {
    char message[MESSAGE_SIZE]={0};
    while (true) {
        recv(client_socket, message, MESSAGE_SIZE, 0);
        if (is_client_connection_closed(message)) {
            break;
        }
        std::cout << message << std::endl;
    }
}
