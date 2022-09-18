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
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    
    // send data to the server
    char server_response[256];
    strcpy(server_response,"Hello from client");
    sendto(network_socket,server_response,sizeof(server_response),0,(struct sockaddr*) &server_address,sizeof(server_address));
    printf("[+] Data sent to server: %s\n",server_response);
    
    // receive data from the server
    int server_length = sizeof(server_address);
    recvfrom(network_socket,&server_response,sizeof(server_response),0,(struct sockaddr*) &server_address, &server_length);
    printf("[+] Data received from server: %s\n",server_response);

    return 0;
}