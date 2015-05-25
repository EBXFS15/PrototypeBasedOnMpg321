#include "fileplayer.h"



filePlayer::filePlayer(QObject *parent) : QObject(parent)
{
    player = new QProcess;
    connect(player, SIGNAL(readyReadStandardOutput()), this, SLOT(player_update()));
    connect(player, SIGNAL(readyReadStandardError()), this, SLOT(player_update()));
    player->start("mpg321", QStringList() << "-R" << "abc");
}

filePlayer::~filePlayer()
{
    player->write("Q");
    player->closeWriteChannel();
    player->waitForFinished();
    delete player;

}

void filePlayer::player_update()
{
    QString msg = player->readAll();
    if(msg.startsWith("@F")){
        emit playbackInfo(msg);
    }
    else{
        if(msg.startsWith("@")){
            emit statusChanged(msg);
        }
    }
}


void filePlayer::loadFile(QString path){
    nextFile = path;
}

void filePlayer::play(){
    /**! unpause if playback is paused and play is called */
    if(paused == true){
        pause();
        return;
    }

    currentFile = nextFile;
    QByteArray cmd = "L ";
    cmd.append(currentFile);
    player->write(cmd);
    paused = false;
    return;
}

void filePlayer::stop(){
    player->write("S");
    currentFile = "";
}

void filePlayer::ff(int frames){
    QByteArray cmd = "J +";
    cmd.append(QString::number(frames));
    player->write(cmd);
}

void filePlayer::rw(int frames){
    QByteArray cmd = "J -";
    cmd.append(QString::number(frames));
    player->write(cmd);
}

void filePlayer::pause(){
    player->write("P");
    paused = !paused;
}
