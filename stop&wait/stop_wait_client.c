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

    // frame declaration
    int frame_id = 0;
    Frame frame_send;
    Frame frame_recv;
    int ack_recv = 1;

    // create the socket
    int network_socket ;
    network_socket = socket(AF_INET,SOCK_DGRAM,0);

    // specify an address for the socket
    struct sockaddr_in server_address, newServer;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

/*
    0- ACK
    1- SEQ
*/

    while (1)
    {
        if(ack_recv == 1){
            // send data to the server
            frame_send.seq_no = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;
            printf("[+] Enter data to send: \n");
            scanf("%s",frame_send.packet.data);
            sendto(network_socket,&frame_send,sizeof(frame_send),0,(struct sockaddr*) &server_address,sizeof(server_address));
            printf("[+] Frame sent to server: %s\n",frame_send.packet.data);
    }

        // receive data from the server
        int server_length = sizeof(server_address);
        int f_recv_size = recvfrom(network_socket,&frame_recv,sizeof(frame_recv),0,(struct sockaddr*) &server_address, &server_length);
        printf("[+] Frame received from server: %s",frame_recv.packet.data);
        if(f_recv_size > 0 && frame_recv.seq_no == 0 && frame_recv.ack == frame_id+1){
            printf("[+] Acknowledgement received from server: %d\n",frame_recv.ack);
            ack_recv = 1;
        }else{
            printf("[+] Acknowledgement not received from server: %d\n",frame_recv.ack);
            ack_recv = 0;
        }
        frame_id++;
    }
    close(network_socket);


}
