#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int Write_multiple_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val);
/*
    server_add: server address
    port: server port
    st_r: starting register
    n_r: number of registers
    regs: array of registers to write

    return: number of registers written or -1 if error
*/

int Read_h_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val);
/*
    server_add: server address
    port: server port
    st_r: starting register
    n_r: number of registers
    regs: array of registers to read

    return: number of registers read or -1 if error
*/