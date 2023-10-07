# ModBus Client

## Description

This is a simple ModBus client that can be used to read and write to ModBus registers. It is written in C and has a representation of the different ModBus layers as the application layer and the transport layer.

- MBclient.c
    > This is the main file where the usage can be configured.

- modbusAP.c
    > This file contains the application layer functions, where the APDU is assembled.

- modbusTCP.c
    > This file contains the transport layer functions, where the MBAP is assembled in case of a write function, or dissembled in case of a read function.


## Usage

First, the client must be configured in the MBclient.c file. The following parameters must be set:

- IP address of the server
- Port of the server
- Write or read function
- Start Register address
- Number of registers
- Data to write (if write function)

Example:
```c
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
´´´

```c
// Data to write
uint16_t DATA[NUMBER_OF_REGISTERS] = {153,69,153,148,159,14,0,86,157,142};
´´´

Then, the client can be compiled and executed with the following commands:

```bash
gcc MBclient.c modbusAP.c modbusTCP.c -o MBclient

./MBclient
```