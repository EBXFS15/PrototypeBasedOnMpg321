#ifndef RFID_H
#define RFID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyO1"/*UART NAME IN PROCESSOR*/
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define BLANK_RFID_TAG "00000000000000\0"

void openport();
void sendport(void);
int readport(char rfidTag[15]);
void closeport(void);


#ifdef __cplusplus
}
#endif

#endif // RFID_H

