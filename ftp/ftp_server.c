#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd){
    int n;
    char buffer[SIZE];
    FILE *fp;
    char *filename = "file2.txt";
    fp = fopen("file2.txt", "w");
    if (fp == NULL) {
        printf("[-]Error in creating file.");
        exit(1);
    }
    while (1) {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0) {
            break;
            return;
        }
        fputs(buffer, fp);
        bzero(buffer, SIZE);
    }

}

int main(int argc, char *argv[])
{
    int portno = atoi(argv[2]);
    int sockfd, new_sock;
    int e;
    struct sockaddr_in servaddr, newaddr;
    char buffer[SIZE];

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

    // bind the socket to our specified IP and port
    e = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (e < 0) {
        printf("[-]bind failed...\n");
        exit(0);
    }
    else
        printf("[+]bind to port %d\n", portno);

    // listen for connections
    e = listen(sockfd, 10);
    if (e < 0) {
        printf("[-]listen failed...\n");
        exit(0);
    }
    else
        printf("[+]Server listening..\n");

    // accept the data packet from client and verification
    int len = sizeof(newaddr);
    new_sock = accept(sockfd, (struct sockaddr*)&newaddr, &len);
    if (new_sock < 0) {
        printf("[-]accept failed...\n");
        exit(0);
    }
    else
        printf("[+]server acccept the client...\n");

    write_file(new_sock);
    printf("[+]File written successfully.\n");
}
