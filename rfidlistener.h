#ifndef RFIDLISTENER_H
#define RFIDLISTENER_H
#include <QObject>
#include <QThread>
#include "rfid.h"

class rfidListener : public QThread
{
    Q_OBJECT


public:    
    rfidListener();
    void run();
    ~rfidListener();

signals:
    void newTagDetected(QString tagId);
};

#endif // RFIDLISTENER_H
