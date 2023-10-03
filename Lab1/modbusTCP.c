#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "modbusTCP.h"

int Send_Modbus_request(char *server_add, int port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R){

    int sock, len;
    struct sockaddr_in serv;
    socklen_t addlen = sizeof(serv);
    int MBAPDU_len = APDUlen + 15;
    unit8_t MBAPDU[MBAPDU_len];
    
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("socket");
        return -1;
    }
    printf("[INFO] Socket created\n");

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_aton(server_add, &serv.sin_addr);

    if(connect(sock, (struct sockaddr *)&serv, addlen) < 0){
        perror("connect");
        return -1;
    }

    printf("[INFO] Connected to %s:%d\n", server_add, port);

    // TI
    MBAPDU[0] = 0;
    MBAPDU[1] = 1;
    // PI
    MBAPDU[2] = 0;
    MBAPDU[3] = 0;
    // length
    MBAPDU[4] = 0;
    MBAPDU[5] = 8 + 1; // unit_ID + APDU
    // unit ID
    MBAPDU[6] = 1;

    // APDU
    // FC
    MBAPDU[7] = 16;
    // Start
    MBAPDU[8] = 0;
    MBAPDU[9] = 1;
    // num reg
    MBAPDU[10] = 0;
    MBAPDU[11] = 1;
    // num bytes
    MBAPDU[12] = 2;
    // reg val
    MBAPDU[13] = 33;
    MBAPDU[14] = 55;

    for(int i=15; i<MBAPDU_len; i++){
        MBAPDU[i] = APDU[i-15];
    }

    if(len = send(sock, MBAPDU, MBAPDU_len, 0) < 0){
        perror("send");
        return -1;
    }

    printf("[INFO] Data sent\n");

    if(len = recv(sock, MBAPDU, MBAPDU_len, 0) < 0){
        perror("recv");
        return -1;
    }

    printf("[INFO] Data received\n");

    close(sock);

    return len;
}


