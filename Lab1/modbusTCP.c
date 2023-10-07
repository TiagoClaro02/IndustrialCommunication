#include "modbusTCP.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>

#define MBAP_len 7

int Send_Modbus_request(char *server_add, int port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R){

    int sock, len;
    struct sockaddr_in serv;
    static int TI=0;
    uint8_t MBAP[MBAP_len];

    // Assemble the MBAP

    TI++;
    // Transaction ID
    MBAP[0] = (uint8_t)((TI & 0xFF00) >> 8);
    MBAP[1] = (uint8_t)(TI & 0xFF);
    // Protocol ID
    MBAP[2] = 0;
    MBAP[3] = 0;
    // length
    MBAP[4] = (uint8_t)((APDUlen+1) >> 8);
    MBAP[5] = (uint8_t)((APDUlen+1) & 0xFF);
    // unit ID
    MBAP[6] = 1;

    printf("MBAP:\nTI:%.2x %.2x\tPI:%.2x %.2x\tlength:%.2x %.2x\tUI:%.2x\n", MBAP[0], MBAP[1], MBAP[2], MBAP[3], MBAP[4], MBAP[5], MBAP[6]);

    // Configure server
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(server_add);

    // Create socket
    printf("[TCP] Creating socket...\n");
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        perror("socket");
        return -1;
    }
    printf("[TCP] Socket created\n");

    // Connect to server
    printf("[TCP] Connecting to %s:%d...\n", server_add, port);
    if(connect(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0){
        perror("connect");
        return -1;
    }

    printf("[TCP] Connected to %s:%d\n", server_add, port);


    // Send MBAP
    printf("[TCP] Sending data...\n");

    len = send(sock, MBAP, MBAP_len, 0);

    if(len < 0){
        perror("[TCP] send write");
        return -1;
    }

    // Send APDU
    len = send(sock, APDU, APDUlen, 0);

    if(len != APDUlen){
        perror("[TCP] len");
        return -1;
    }

    printf("[TCP] Data sent\n");

    printf("[TCP] Receiving data...\n");

    // Receive MBAP
    len = recv(sock, MBAP, MBAP_len, 0);

    if(len < 0){
        perror("recv");
        return -1;
    }

    printf("[TCP] MBAP received\n");

    // Get length of APDU
    len = (MBAP[4] << 8) + MBAP[5];
    
    // Receive APDU
    int len_response = recv(sock, APDU_R, len-1, 0);

    if(len_response < 0){
        perror("TCP read");
        return -1;
    }

    printf("[TCP] APDU received\n");

    printf("[TCP] Closing socket...\n");
    close(sock);
    printf("[TCP] Socket closed\n");

    return 0;
}


