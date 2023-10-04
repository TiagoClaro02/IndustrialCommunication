#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "modbusAP.h"

#define IP "127.0.0.1"
#define PORT 502

int main(){

    int sd;
    unsigned char buf[256];

    struct sockaddr_in socket_addr;

    printf("[INFO] Creating socket...\n");
    sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("[INFO] Socket created\n");

    if(sd < 0){
        perror("[ERROR socket");
        return -1;
    }

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(PORT);
    inet_aton(IP, &socket_addr.sin_addr);

    printf("[INFO] Connecting to %s:%d...\n", IP, PORT);
    if(connect(sd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0){
        perror("[ERROR] connect");
        return -1;
    }
    printf("[INFO] Connected to %s:%d\n", IP, PORT);

    bzero(buf, sizeof(buf));
    printf("\n Input data to send: ");
    //scanf("%s", buf);
    fgets(buf, sizeof(buf), stdin);

    printf("\n[INFO] Sending data...\n");
    if(send(sd, buf, strlen(buf), 0) < 0){
        perror("[ERROR] send");
        return -1;
    }
    printf("[INFO] Data sent\n");

    printf("[INFO] Closing socket...\n");
    close(sd);

    printf("[INFO] Socket closed\n");

    return 0;
}