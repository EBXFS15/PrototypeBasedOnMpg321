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
#include "hardware.h"

#define FALSE 0
#define TRUE 1

/*!
 * \brief Baudrate the RFID reader is using by default.
 */
#define BAUDRATE B9600

/*!
 * \brief Port where the RFID reader is connected to.
 */
#define MODEMDEVICE_BBB "/dev/ttyO0"
#define MODEMDEVICE_PI3 "/dev/ttyAMA0"

/*!
 * \brief Initial tag id for detection.
 * TAG-ID that is unexpected to be used. To be checked based on the used tags.
 */
#define BLANK_RFID_TAG "00000000000000\n"
#define RFID_SIZE 15
#define RFID_LEN  14


/*!
 * \brief open the serial port
 * Very simple implementation to access the UART port.
 */

/*!
 * \brief openport the serial port
 * \param device 0x01 for BBB, 0x02 for PI3
 * Very simple implementation to access the UART port.
 */
void openport(Hardware hwType);

/*!
 * \brief read from port in a blocking manner.
 *
 * \param rfidTag character array where the reseived string shall be stored in.
 */
int readport(char rfidTag[15]);
/*!
 * \brief closes the serial port
 *
 */
void closeport(void);


#ifdef __cplusplus
}
#endif

#endif // RFID_H

