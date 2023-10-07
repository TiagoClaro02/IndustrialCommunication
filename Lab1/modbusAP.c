#include "modbusAP.h"
#include "modbusTCP.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define FCODE_WRITE_MULTIPLE_REGS 0x10
#define FCODE_READ_MULTIPLE_REGS 0x03

int Write_multiple_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val){

    uint8_t APDU[6 + n_r*2];
    uint8_t APDU_R[5];
    int APDU_len;

    // Check for errors
    if(st_r + n_r > 0xFFFF){
        perror("st_r + n_r > 0xFFFF");
        return -1;
    }
    if(n_r > 0x007B || n_r < 1){
        perror("n_r > 0x007B || n_r < 1");
        return -1;
    }
    if(val == NULL){
        perror("val == NULL");
        return -1;
    }

    // Assemble the APDU

    printf("[AP] Creating write APDU...\n");

    // Function code
    APDU[0] = FCODE_WRITE_MULTIPLE_REGS;
    // Starting register
    APDU[1] = (uint8_t)(st_r >> 8);
    APDU[2] = (uint8_t)(st_r & 0x00FF);
    // Number of registers
    APDU[3] = (uint8_t)(n_r >> 8);
    APDU[4] = (uint8_t)(n_r & 0x00FF);
    // Number of bytes
    APDU[5] = (uint8_t)(n_r*2);
    // Values
    for(int i=0; i<n_r; i++){
        APDU[6+i*2] = (uint8_t)(val[i] >> 8);
        APDU[7+i*2] = (uint8_t)(val[i] & 0x00FF);
    }
    // APDU length
    APDU_len = 6 + n_r*2;


    // Send the APDU
    printf("[AP] Sending APDU...\n");

    if(Send_Modbus_request(server_add, port, APDU, APDU_len, APDU_R) != 0){
        perror("Send_Modbus_request");
        return -1;
    }

    printf("[AP] APDU sent\n");

    return (int)n_r;
}

int Read_h_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val){

    int APDU_len = 5;
    uint8_t APDU[APDU_len];
    uint8_t APDU_R[2 + n_r*2];

    // Check for errors
    if(st_r + n_r > 0xFFFF){
        perror("st_r + n_r > 0xFFFF");
        return -1;
    }
    if(n_r > 0x007B || n_r < 1){
        perror("n_r > 0x007B || n_r < 1");
        return -1;
    }

    // Assemble the APDU

    printf("[AP] Creating write APDU...\n");

    // Function code
    APDU[0] = FCODE_READ_MULTIPLE_REGS;
    // Starting register
    APDU[1] = (uint8_t)(st_r >> 8);
    APDU[2] = (uint8_t)(st_r & 0x00FF);
    // Number of registers
    APDU[3] = (uint8_t)(n_r >> 8);
    APDU[4] = (uint8_t)(n_r & 0x00FF);

    // Send the APDU
    printf("[AP] Sending APDU...\n");

    if(Send_Modbus_request(server_add, port, APDU, APDU_len, APDU_R) != 0){
        perror("Send_Modbus_request");
        return -1;
    }

    // Check the response
    for(int i=0; i<n_r; i++){
        val[i] = (int16_t)(APDU_R[2+i*2] << 8) | APDU_R[3+i*2];
    }

    printf("[AP] APDU sent\n");

    return (int)n_r;
}

