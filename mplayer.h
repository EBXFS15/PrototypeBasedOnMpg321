#ifndef MPLAYER_H
#define MPLAYER_H

#include <QObject>
#include <QProcess>

class mplayer : public QObject
{
    Q_OBJECT
    QProcess * player;
    QString currentFile;
    QString nextFile;
    bool paused;

public:
    explicit mplayer(QObject *parent = 0);
    ~mplayer();

signals:
    void playbackInfo(QString filename);
    void statusChanged(QString newStatus);
    void playbackEnded();
    void playbackPosition(int);

public slots:
    void loadFile(QString path);
    void play();
    void stop();
    void pause();
    void ff(int frames);
    void rw(int frames);

private slots:
    void player_update();
    void sendCommandToPlayer(QByteArray cmd);
};

#endif // MPLAYER_H
