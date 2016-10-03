#ifndef CONTINUETOPLAY_H
#define CONTINUETOPLAY_H

#include <QObject>
#include <QSettings>
#include <QSettings>

#define FILE_NAME_STARTUP_CONFIG ".ebxfs15_musicplayer_startup"

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
    void load(void);

private:
    QSettings *settings;
    bool loaded;
};

#endif // CONTINUETOPLAY_H
