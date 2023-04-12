#include <iostream>
#include <string.h>

class Buffer{
    private:
    std::string msg;

    public:
    Buffer();
    void to_empty();
    void enter_name(std::string value);
    void enter_message(char* value);
    std::string get_message();
};