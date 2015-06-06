#ifndef RFIDLISTENER_H
#define RFIDLISTENER_H
#include <QObject>
#include <QThread>
#include "rfid.h"

class rfidListener : public QObject
{
    Q_OBJECT

public:    
    rfidListener();
    ~rfidListener();

public slots:
    void process();

signals:
    void newTagDetected(QString tagId);

};

#endif // RFIDLISTENER_H
