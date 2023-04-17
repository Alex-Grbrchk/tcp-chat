# tcp-chat

Server:

make -f Makefile-1

Client:

make -f Makefile-2

# Brief describtion of project architecture

The "errors.h" file stores wrapper functions declarations of the main functions for working with sockets. Wrapper functions are implemented for more convenient error output.
Class "Client" is implemented to store the name of the connected client. It contains a name field and two methods. 
Class "Buffer" is created for simplified work with the client's message. It contains a message field and three methods for creation of final message to send.

Server.cpp:

Function is_client_connection_closed checks whether the connection termination symbol has been entered in the message. 
Sockets of connected clients are placed in the vector client_sockets. This vector is created for forwarding messages and further termination of the client connection.
Function handle_client processes the client's message and forwards it to the other connected clients. This function is executed continuously in a separate thread.

Chat.cpp:

After establishing a connection with the server, an object of the Client class is created. Connected client enters their name. Next, threads of receiving and sending messages are created. 
Function receive_messages receives other clients messages from server. 
Function send_messages checks the readability of data every second. If the data is available, reads the message from the console, checks for the presence of a connection termination symbol. Next, an object of the Buffer class is created. Using the methods of the Buffer class, a final message is generated for sending, which consists of the client name and the entered message.
