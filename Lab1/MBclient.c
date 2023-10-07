#include "modbusAP.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

// Server address and port
#define IP "127.0.0.1"
#define PORT 502

// Function codes
#define WRITE 0x10
#define READ 0x03

// Data to input
#define FUNCTION_CODE READ
#define STARTING_REGISTER 0
#define NUMBER_OF_REGISTERS 10

int main(){

    // Data to input
    uint16_t DATA[NUMBER_OF_REGISTERS] = {153,69,153,148,159,14,0,86,157,142};

    if(FUNCTION_CODE == WRITE){
        // Write
        uint16_t val[NUMBER_OF_REGISTERS];
        for(int i=0; i<NUMBER_OF_REGISTERS; i++){
            val[i] = (uint16_t)DATA[i];
        }
        if(Write_multiple_regs(IP, PORT, STARTING_REGISTER, NUMBER_OF_REGISTERS, val) < 0){
            perror("Write_multiple_regs");
            return -1;
        }
    }
    else if(FUNCTION_CODE == READ){
        // Read
        uint16_t val[NUMBER_OF_REGISTERS];
        if(Read_h_regs(IP, PORT, STARTING_REGISTER, NUMBER_OF_REGISTERS, val) < 0){
            perror("Read_h_regs");
            return -1;
        }
        printf("Read per Register:\n");
        for(int i=0; i<NUMBER_OF_REGISTERS; i++){
            if(i== NUMBER_OF_REGISTERS-1){
                printf("[%d] %d", STARTING_REGISTER+i, val[i]);
                break;
            }
            printf("[%d] %d | ", STARTING_REGISTER+i, val[i]);
        }
        printf("\n");
    }
    else{
        perror("Invalid function code");
        return -1;
    }

    return 0;
}