#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 502
#define MAX_BUF_SIZE 256

int main(){

    int sd, client_sd, i = 0;
    unsigned char buf[MAX_BUF_SIZE], data[1];
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_len = sizeof(server_addr);

    printf("[INFO] Creating socket...\n");
    sd = socket(AF_INET, SOCK_STREAM, 0);

    if(sd < 0){
        perror("[ERROR] socket");
        return -1;
    }
    printf("[INFO] Socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(IP, &server_addr.sin_addr);

    printf("[INFO] Binding to %s:%d...\n", IP, PORT);
    if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("[ERROR] bind");
        return -1;
    }
    printf("[INFO] Binded to %s:%d\n", IP, PORT);

    printf("[INFO] Listening...\n");
    if(listen(sd, 1) < 0){
        perror("[ERROR] listen");
        return -1;
    }
    
    printf("[INFO] Waiting for client...\n");
    client_sd = accept(sd, (struct sockaddr *)&client_addr, &server_len);

    if(client_sd < 0){
        perror("[ERROR] accept");
        return -1;
    }
    printf("[INFO] Client connected\n");

    printf("[INFO] Receiving data...\n");

    while(1){
        if(recv(client_sd, data, 1, 0) < 0){
            perror("[ERROR] recv");
            return -1;
        }
        if(data[0] == '\n'){
            break;
        }
        buf[i] = data[0];
        i++;

        if(i >= MAX_BUF_SIZE){
            printf("[ERROR] Maximum buffer size exceeded\n");
            return -1;
        }
    }

    buf[i] = '\0'; // Add null character at the end of the buffer

    printf("[INFO] Data received\n");

    printf("[INFO] Data: %s\n", buf);

    printf("[INFO] Closing socket...\n");

    close(sd);

    printf("[INFO] Socket closed\n");

    return 0;

}