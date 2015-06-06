#ifndef MPLAYER_H
#define MPLAYER_H

/*!
 * \brief Building variable to switch between video support and no video support
 * For the moment video is not supported.
 */
#define NO_VIDEO

#include <QObject>
#include <QProcess>
#include <QHash>
#include <QWidget>

class mplayer : public QObject
{
    Q_OBJECT
    QProcess * player;
    QString currentFile;
    QString nextFile;
    bool paused;
    int seeking;

public:
    typedef struct{
        QString artist;
        QString album;
        QString title;
    }MediaInfo;
    MediaInfo m_mediaInfo;

public:
    explicit mplayer(QObject *parent = 0);
    ~mplayer();

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
    void startProcess(int winId=0);

private slots:
    void player_update();
    void parsePosition(const QString &line);
    void parseLine(const QString &line);
    void sendCommandToPlayer(QString cmd);
};

#endif // MPLAYER_H
