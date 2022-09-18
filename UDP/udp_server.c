#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]){
    int portno = atoi(argv[2]);

    if(argc < 3){
        printf("Usage: %s <ip> <port>",argv[0]);
        exit(0);
    }

    // create the socket
    int network_socket ;
    network_socket = socket(AF_INET,SOCK_DGRAM,0);

    // specify an address for the socket
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    
    // bind the socket to our specified IP and port
    bind(network_socket,(struct sockaddr*) &server_address,sizeof(server_address));

    // receive data from the client
    char server_response[256];
    int client_length = sizeof(client_address);
    recvfrom(network_socket,&server_response,sizeof(server_response),0,(struct sockaddr*) &client_address, &client_length);
    printf("[+] Data received from client: %s\n",server_response);

    //send data to the client
    sendto(network_socket,server_response,sizeof(server_response),0,(struct sockaddr*) &client_address,sizeof(client_address));
    printf("[+] Data sent to client: %s\n",server_response);

    return 0;
}