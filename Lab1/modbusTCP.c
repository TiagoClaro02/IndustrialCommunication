#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "modbusTCP.h"

#define MBAPDU_len 7

int Send_Modbus_request(char *server_add, int port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R){

    int sock, MBAPDU_R_len, APDU_R_len, len;
    struct sockaddr_in serv;
    socklen_t addlen = sizeof(serv);
    int PDU_len = MBAPDU_len + APDUlen;
    int TI=0;
    unsigned char MBAPDU[MBAPDU_len];
    unsigned char MBAPDU_R[MBAPDU_len];
    unsigned char PDU[PDU_len];
    
    printf("[INFO] Creating socket...\n");
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("socket");
        return -1;
    }
    printf("[INFO] Socket created\n");

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_aton(server_add, &serv.sin_addr);

    printf("[INFO] Connecting to %s:%d...\n", server_add, port)
    if(connect(sock, (struct sockaddr *)&serv, addlen) < 0){
        perror("connect");
        return -1;
    }

    printf("[INFO] Connected to %s:%d\n", server_add, port);

    // TI
    MBAPDU[0] = (uint8_t) (TI>>8);
    MBAPDU[1] = (uint8_t) (TI & 0xff);
    // PI
    MBAPDU[2] = 0x00;
    MBAPDU[3] = 0x00;
    // length
    MBAPDU[4] = (uint8_t) ((APDUlen+1)>>8);
    MBAPDU[5] = (uint8_t) ((APDUlen+1) & 0xff);
    // unit ID
    MBAPDU[6] = 0x01;

    for(int i=0; i<MBAPDU_len; i++){
        PDU[i] = MBAPDU[i];
    }
    for(int i=MBAPDU_len; i<PDU_len; i++){
        PDU[i] = APDU[i-MBAPDU_len];
    }

    printf("[INFO] Sending data...\n");
    if(len = send(sock, PDU, PDU_len, 0) < 0){
        perror("send");
        return -1;
    }

    printf("[INFO] Data sent\n");

    printf("[INFO] Receiving data...\n");

    if(MBAPDU_R_len = recv(sock, MBAPDU_R, MBAPDU_len, 0) < 0){
        perror("recv");
        return -1;
    }

    printf("[INFO] MBAPDU received\n");

    if(MBAPDU_R_len != MBAPDU_len){
        printf("[ERROR] Wrong MBAPDU length\n");
        return -1;
    }
    if(MBAPDU_R[0] != 0x15 || MBAPDU_R[1] != 0x01){
        printf("[ERROR] Wrong TI\n");
        return -1;
    }
    if(MBAPDU_R[2] != 0x00 || MBAPDU_R[3] != 0x00){
        printf("[ERROR] Wrong PI\n");
        return -1;
    }

    APDU_R_len = MBAPDU_R[4] * 256 + MBAPDU_R[5];

    if(len = recv(sock, APDU_R, APDU_R_len, 0) < 0){
        perror("recv");
        return -1;
    }

    printf("[INFO] APDU received\n");

    printf("[INFO] Closing socket...\n");
    close(sock);
    printf("[INFO] Socket closed\n");

    return 0;
}


