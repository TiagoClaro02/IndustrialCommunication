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

    Write_multiple_regs(IP, PORT, 0, 2, (uint8_t *) "\x00\x01\x00\x02");

    return 0;
}