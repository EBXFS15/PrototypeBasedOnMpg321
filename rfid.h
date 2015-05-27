#ifndef RFID_H
#define RFID_H

#include <QObject>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

class rfid : public QObject
{
    Q_OBJECT
    int serialPortFileDescriptor;

public:
    explicit rfid(QObject *parent = 0);
    ~rfid();

signals:
    void newIdReceived(QString filename);

public slots:
    void listen();
};

#endif // RFID_H
