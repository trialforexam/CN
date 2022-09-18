#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            printf("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main(int argc, int *argv[]){
    int portno = atoi(argv[2]);
    int sockfd;
    int e;
    struct sockaddr_in servaddr;
    char buffer[SIZE];
    FILE *fp;
    char *filename = "file1.txt";

    if (argc != 3) {
        printf("Usage: %s <ip> <port>",argv[0]);
        exit(0);
    }

    // create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("[-]socket creation failed...\n");
        exit(0);
    }
    else
        printf("[+]Socket successfully created..\n");

    // specify an address for the socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(portno);

    //connect to the server
    e = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (e < 0) {
        printf("[-]connection failed...\n");
        exit(0);
    }
    else
        printf("[+]connected to the server..\n");

    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("[-]Error in reading file.\n");
        exit(0);
    }

    // send the file
    send_file(fp, sockfd);
    printf("[+]File data sent successfully.\n");

    // close the socket
    close(sockfd);
    printf("[+]Disconnected from the server.\n");

    return 0;
}