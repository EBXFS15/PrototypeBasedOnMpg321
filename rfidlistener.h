#ifndef RFIDLISTENER_H
#define RFIDLISTENER_H
#include <QObject>
#include <QThread>
#include "rfid.h"
#include "hardware.h"

class rfidListener : public QObject
{
    Q_OBJECT

public:    
    rfidListener(Hardware hwType);
    ~rfidListener();

public slots:
    /*!
     * \brief Reads form the UART in a blocking manner.
     */
    void process();

signals:
    /*!
     * \brief signalization if a new tag was discovered.
     * last discovered tag does not issue a signal.
     * \param tagId TAG-ID of the newly discovered tag.
     */
    void newTagDetected(QString tagId);

};

#endif // RFIDLISTENER_H
