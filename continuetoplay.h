#ifndef CONTINUETOPLAY_H
#define CONTINUETOPLAY_H

#include <QObject>
#include <QSettings>
#include <QSettings>
#include <QDir>
#define CONFIG_ROOT_PATH "/var/tmp/"
#define CONFIG_FILE_ROOT_PATH CONFIG_ROOT_PATH "musicplayer/"
#define FILE_NAME_STARTUP_CONFIG CONFIG_FILE_ROOT_PATH ".ebxfs15_musicplayer_startup"

class ContinueToPlay : public QObject
{
    Q_OBJECT
public:
    explicit ContinueToPlay(QObject *parent = 0);
    ~ContinueToPlay();

signals:
    void startPlayback(QString playlist, int track, int position);

public slots:
    void setCurrentPlaylist(QString playlist);
    void setCurrentTrack(int track);
    void setPlaybackPosition(int position);
    void store(void);
    void load(void);

private:
    QSettings *settings;
    bool loaded;
    void createConfigEnvironment();

};

#endif // CONTINUETOPLAY_H
