#include <iostream>
#include <string.h>

class Client{
    private:
    std::string name;
    
    public:
    Client();
    void input_name();
    std::string get_name();
    Client& operator= (std::string value);
    

};