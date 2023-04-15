# tcp-chat
Simple tcp chat 

Build steps for server:

1. g++ -c errors.cpp
2. g++ -c Server.cpp
3. g++ -o server errors.o Server.o -lpthread

Biuld steps for client:

1. g++ -c Buffer.cpp
2. g++ -c Client.cpp
3. g++ -c Chat.cpp
4. g++ -o client errors.o Buffer.o Client.o Chat.o -lpthread
