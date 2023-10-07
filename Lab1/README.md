# ModBus Client

## Description

This is a simple ModBus client that can be used to read and write to ModBus registers. It is written in C and has a representation of the different ModBus layers as the application layer and the transport layer.

- MBclient.c
    > This is the main file where the usage can be configured.

- modbusAP.c
    > This file contains the application layer functions, where the APDU is assembled.

- modbusTCP.c
    > This file contains the transport layer functions, where the MBAP is assembled in case of a write function, or dissembled in case of a read function.