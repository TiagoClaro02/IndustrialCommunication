#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "modbusTCP.h"

int Write_multiple_regs(char *server_add, int port, int st_r, int n_r, uint8_t *regs);
/*
    server_add: server address
    port: server port
    st_r: starting register
    n_r: number of registers
    regs: array of registers to write

    return: number of registers written or -1 if error
*/

int Read_h_regs(char *server_add, int port, int st_r, int n_r, uint8_t *regs);
/*
    server_add: server address
    port: server port
    st_r: starting register
    n_r: number of registers
    regs: array of registers to read

    return: number of registers read or -1 if error
*/