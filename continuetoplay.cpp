#include "continuetoplay.h"

ContinueToPlay::ContinueToPlay(QObject *parent) : QObject(parent)
{
    //QSettings(Scope scope, const QString & organization, const QString & application = QString(), QObject * parent = 0)
    createConfigEnvironment();
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

void ContinueToPlay::store(void)
{
    settings->sync();
}

void ContinueToPlay::setPlaybackPosition(int position)
{
    if (loaded)
    {
        settings->setValue("position", position);        
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

void ContinueToPlay::createConfigEnvironment()
{
    if (!QDir(CONFIG_ROOT_PATH).exists())
    {
        QDir().mkdir(CONFIG_ROOT_PATH);
    }
    if (!QDir(CONFIG_FILE_ROOT_PATH).exists())
    {
        QDir().mkdir(CONFIG_FILE_ROOT_PATH);
    }
}

void ContinueToPlay::load()
{
    //if (settings->contains("playlist") && (loaded == false))
    if (settings->contains("playlist") )
    {                
        QString playlist = "";
        int track = 0;
        int position = 0;
        track = settings->value("track",track).toInt();
        if (track < 0)
        {
            track = 0;
        }
        position = settings->value("position").toInt();
        if (position < 0)
        {
            position = 0;
        }
        playlist = settings->value("playlist",playlist).toString();
        emit startPlayback(playlist, track, position);
    }
    loaded = true;
}

ContinueToPlay::~ContinueToPlay()
{
    delete settings;
}

