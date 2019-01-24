#ifndef HAL_H
#define HAL_H

#include <QObject>

class hal : public QObject
{
    Q_OBJECT
public:
    explicit hal(QObject *parent = 0);
    ~hal();

signals:

public slots:
};

#endif // HAL_H
