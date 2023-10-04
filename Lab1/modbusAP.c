#include "modbusAP.h"
#include "modbusTCP.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define FCODE_WRITE_MULTIPLE_REGS 0x10
#define FCODE_READ_MULTIPLE_REGS 0x03

int Write_multiple_regs(char *server_add, int port, int st_r, int n_r, uint8_t *regs){

    unsigned char APDU[256];
    unsigned char APDU_R[256];
    int APDU_R_len;

    printf("[AP] Creating write APDU...\n");

    APDU[0] = FCODE_WRITE_MULTIPLE_REGS;
    APDU[1] = (uint8_t) (st_r>>8);
    APDU[2] = (uint8_t) (st_r & 0xff);
    APDU[3] = (uint8_t) (n_r>>8);
    APDU[4] = (uint8_t) (n_r & 0xff);

    for(int i=0; i<n_r*2; i++){
        APDU[5+i] = regs[i];
    }

    printf("[AP] Sending APDU...\n");

    if(Send_Modbus_request(server_add, port, APDU, 5+n_r*2, APDU_R) != 0){
        perror("Send_Modbus_request");
        return -1;
    }

    printf("[AP] APDU sent\n");

    return n_r;
}

int Read_h_regs(char *server_add, int port, int st_r, int n_r, uint8_t *regs){

    unsigned char APDU[256];
    unsigned char APDU_R[256];
    int APDU_R_len;

    printf("[AP] Creating read APDU...\n");

    APDU[0] = FCODE_READ_MULTIPLE_REGS;
    APDU[1] = (uint8_t) (st_r>>8);
    APDU[2] = (uint8_t) (st_r & 0xff);
    APDU[3] = (uint8_t) (n_r>>8);
    APDU[4] = (uint8_t) (n_r & 0xff);

    for(int i=0; i<n_r*2; i++){
        APDU[5+i] = regs[i];
    }

    printf("[AP] Sending APDU...\n");

    if(Send_Modbus_request(server_add, port, APDU, 5+n_r*2, APDU_R) != 0){
        perror("Send_Modbus_request");
        return -1;
    }

    printf("[AP] APDU sent\n");

    return n_r;
}

