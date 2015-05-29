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
    int seeking;

public:
    enum State {
        NotStartedState = -1,
        IdleState,
        LoadingState,
        StoppedState,
        PlayingState,
        BufferingState,
        PausedState,
        ErrorState
    };

public:
    explicit mplayer(QObject *parent = 0);
    ~mplayer();

    State m_state;

signals:
    void playbackInfo(QString filename);
    void statusChanged(QString newStatus);
    void playbackEnded();
    void playbackPosition(int);
    void playbackStarted();

public slots:
    void loadFile(QString path);
    void play();
    void stop();
    void pause();
    void ff(int frames);
    void rw(int frames);
    void setVolume(int value);

private slots:
    void player_update();    
    void sendCommandToPlayer(QString cmd);
};

#endif // MPLAYER_H
