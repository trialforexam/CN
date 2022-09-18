#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    char server_message[256] = "You have reached the server!";
    int portno = atoi(argv[2]);

    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);

    // define the server address
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP and port
    bind(server_socket,(struct sockaddr*) &server_address,sizeof(server_address));
    
    // listen for connections
    listen(server_socket,5);

    // accept the connection
    int client_socket;
    int client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_length);

    // send the message
    send(client_socket,server_message,sizeof(server_message),0);

    // close the socket
    close(server_socket);

    return 0;
}