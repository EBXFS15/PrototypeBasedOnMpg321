#include "rfid.h"

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int open_port(void)
{
  int fd; /* File descriptor for the port */


  fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyO1ddd,mn - ");
  }
  else
    fcntl(fd, F_SETFL, 0);

  struct termios options;

  /*
   * Get the current options for the port...
   */

  tcgetattr(fd, &options);

  /*
   * Set the baud rates to 19200...
   */

  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);

  /*
   * Enable the receiver and set local mode...
   */

  options.c_cflag |= (CLOCAL | CREAD);

  /*
   * Set the new options for the port...
   */

  tcsetattr(fd, TCSANOW, &options);
  return (fd);
}


rfid::rfid(QObject *parent) : QObject(parent)
{
    serialPortFileDescriptor = -1;
    serialPortFileDescriptor = open_port();
    if(serialPortFileDescriptor!=-1){
        write(serialPortFileDescriptor,"c",1);
        listen();
    }
}

rfid::~rfid()
{
    if(serialPortFileDescriptor!=-1){
        close(serialPortFileDescriptor);
    }
}

void rfid::listen()
{
    if(serialPortFileDescriptor != -1){
        char rxBuffer[500]={0};
        read(serialPortFileDescriptor,rxBuffer,1);
        emit newIdReceived(rxBuffer);
    }
}

