#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int seq_no;
    int frame_kind; //ACK:0, SEQ:1 FIN:2
    int ack;
    Packet packet;
}Frame;


void main(int argc, char *argv[]){
    char buffer[256];
    int portno = atoi(argv[2]);

    if(argc < 3){
        printf("Usage: %s <ip> <port>",argv[0]);
        exit(0);
    }

    // create the socket
    int network_socket ;
    network_socket = socket(AF_INET,SOCK_DGRAM,0);

    // specify an address for the socket
    struct sockaddr_in server_address, newServer;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    
    // bind the socket to our specified IP and port
    bind(network_socket,(struct sockaddr*) &server_address,sizeof(server_address));
    int newServerLength = sizeof(newServer);

    // declare frame
    int frame_id = 0;
    Frame frame_send;
    Frame frame_recv;


    while(1){
        // recieve frame from client
        int frame_recv_length = recvfrom(network_socket,&frame_recv,sizeof(frame_recv),0,(struct sockaddr*) &newServer, &newServerLength);
        if(frame_recv_length>0 && frame_recv.frame_kind == 1 && frame_recv.seq_no == frame_id){
            printf("[+] Frame received from client: %s\n",frame_recv.packet.data);
            // send ack to client
            frame_send.seq_no = 0;
            frame_send.frame_kind = 0;
            frame_send.ack = frame_recv.seq_no+1;

            sendto(network_socket,&frame_send,sizeof(frame_send),0,(struct sockaddr*) &newServer,sizeof(newServer));
            printf("[+] Ack sent to client: %d\n",frame_send.ack);
        }
        else{
            printf("[-] Frame not received from client\n");
        }
        frame_id++;
    }
    close(network_socket);
}
