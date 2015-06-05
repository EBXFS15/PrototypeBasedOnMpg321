/**
 ***************************************************************************
 * @brief   CAS EBX-FS15 Embedded Linux \n
 *	    Implements UART functions, e.g.: \n
 *	    openport(); / readport(); \n
 * @file    rfid.h
 * @version 1.0
 * @date    04.06.2015
 * @author  Cedric Ocaña
 *
 * @remark  Last Modifications:
 * @remark  V1.0, co, 04.06.2015
 ***************************************************************************
 */
#include "rfid.h"

int fd=0, n;
unsigned char *var;
struct termios oldtp, newtp;
char sendcmd1[10]="\0";
FILE *file;


int readport(char rfidTag[15])
{
    static char internalRfidTag[15]=BLANK_RFID_TAG;
    static char tempRfidTag[15]    =BLANK_RFID_TAG;
    while (fd!=0){
        read(fd,tempRfidTag, sizeof(tempRfidTag));
        tempRfidTag[14] = '\0';
        if(tempRfidTag[0] == '\n'){
            tempRfidTag[0] = '0';
        }
        if (strcmp(tempRfidTag,internalRfidTag) != 0){
            //printf("New Tag: %s\n", tempRfidTag);
            strcpy(internalRfidTag, tempRfidTag);
            strcpy(rfidTag, tempRfidTag);
            return 1;
        }
    }
    return 0;
}
void openport()
{

     fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
         if (fd <0)
         {
            perror(MODEMDEVICE);
         }

         fcntl(fd,F_SETFL,0);
         tcgetattr(fd,&oldtp); /* save current serial port settings */
    //     tcgetattr(fd,&newtp); /* save current serial port settings */
         bzero(&newtp, sizeof(newtp));
  //       bzero(&oldtp, sizeof(oldtp));

         newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

         newtp.c_iflag = IGNPAR | ICRNL;

         newtp.c_oflag = 0;
         newtp.c_lflag = ICANON;
         newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
         newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
         newtp.c_cc[VERASE]   = 0;     /* del */
         newtp.c_cc[VKILL]    = 0;     /* @ */
        // newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
         newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
         newtp.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
         newtp.c_cc[VSWTC]    = 0;     /* '\0' */
         newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
         newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
         newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
         newtp.c_cc[VEOL]     = 0;     /* '\0' */
         newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
         newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
         newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
         newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
         newtp.c_cc[VEOL2]    = 0;     /* '\0' */

//	  tcflush(fd, TCIFLUSH);
//	 tcsetattr(fd,TCSANOW,&newtp);

}
void closeport(void)
{
    int tmpFp = fd;
    fd = 0;
    close(tmpFp);
}
