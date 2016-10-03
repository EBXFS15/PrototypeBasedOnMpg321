#include "continuetoplay.h"

ContinueToPlay::ContinueToPlay(QObject *parent) : QObject(parent)
{
    //QSettings(Scope scope, const QString & organization, const QString & application = QString(), QObject * parent = 0)
    settings = new QSettings(FILE_NAME_STARTUP_CONFIG, QSettings::NativeFormat, parent);
    loaded = false;
}

void ContinueToPlay::setCurrentTrack(int track)
{
    if (loaded)
    {
        settings->setValue("track",track);
        setPlaybackPosition(0);
    }
}

void ContinueToPlay::setPlaybackPosition(int position)
{
    if (loaded)
    {
        settings->setValue("position", position);
        settings->sync();
    }
}

void ContinueToPlay::setCurrentPlaylist(QString playlist)
{
    if (loaded)
    {
        settings->setValue("playlist", playlist);
        setCurrentTrack(0);
    }
}

void ContinueToPlay::load()
{
    if (settings->contains("playlist"))
    {
        QString playlist = "";
        int track = 0;
        int position = 0;
        track = settings->value("track",track).toInt();
        position = settings->value("position").toInt();
        playlist = settings->value("playlist",playlist).toString();
        emit startPlayback(playlist, track, position);
    }
    loaded = true;
}

ContinueToPlay::~ContinueToPlay()
{
    delete settings;
}

