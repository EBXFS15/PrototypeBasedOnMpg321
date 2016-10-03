#ifndef CONTINUETOPLAY_H
#define CONTINUETOPLAY_H

#include <QObject>

class ContinueToPlay : public QObject
{
    Q_OBJECT
public:
    explicit ContinueToPlay(QObject *parent = 0);
    ~ContinueToPlay();

signals:

public slots:
};

#endif // CONTINUETOPLAY_H
