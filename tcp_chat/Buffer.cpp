#include "Buffer.h"

Buffer::Buffer(){

}

void Buffer::enter_name(std::string value){
    msg = value + ": ";
}

void Buffer::to_empty() {
    msg = "";
}

void Buffer::enter_message(std::string value){
    msg += value;
}

std::string Buffer::get_message(){
    return msg;
}
