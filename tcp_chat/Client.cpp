#include "Client.h"

Client::Client(){

}

void Client::input_name(){
    std::getline(std::cin, name);

}

std::string Client::get_name(){
    return name;
}

Client& Client::operator=(std::string value) {
    this->name = value;
    return *this;
}