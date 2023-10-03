#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


int Send_Modbus_request(char *server_add, int port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R);


