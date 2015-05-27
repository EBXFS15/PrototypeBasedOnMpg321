#include "mplayer.h"
#include <QDir>



mplayer::mplayer(QObject *parent) : QObject(parent)
{
    player = new QProcess;
    connect(player, SIGNAL(readyReadStandardOutput()), this, SLOT(player_update()));
    connect(player, SIGNAL(readyReadStandardError()), this, SLOT(player_update()));
    player->start("mplayer", QStringList() << "-slave"  << "-input" << "nodefault-bindings" << "-noconfig" << "all"  << "-idle");
}

mplayer::~mplayer()
{
    sendCommandToPlayer("stop");
    sendCommandToPlayer("quit");
    player->closeWriteChannel();
    player->waitForFinished();
    delete player;

}

void mplayer::player_update()
{
    QString msg = player->readAll();
    QString playBackInfo = msg.mid(msg.lastIndexOf("A:"));
    if(playBackInfo.startsWith("A:") && playBackInfo.endsWith("\r")){
        QRegExp rx("(\\d{1,}.\\d{1,}\\s)");
        QStringList list;
        int pos = 0;

        while ((pos = rx.indexIn(playBackInfo, pos)) != -1) {
            list << rx.cap(1);
            pos += rx.matchedLength();
        }        
        if (list.count() == 2){
            float currentPostion = list[0].toFloat();
            float duration = list[1].toFloat();
            if ((duration - currentPostion) < 0.3){                
                stop();
                emit playbackPosition(100);
                emit playbackEnded();
            }
            else{
                int position = (currentPostion / duration)*100;
                emit playbackPosition(position);
            }
        }
    }
    else{
        if(!msg.startsWith("[")){
            emit statusChanged(msg);
        }
    }
}

/*! Adds a line-feed to the QByteArray in order to trigger the player to start interpretation of cmd*/
void mplayer::sendCommandToPlayer(QString cmd){
    cmd.append("\n");
    player->write(cmd.toLocal8Bit());
}

/*! defines the file to play when the play function is called*/
void mplayer::loadFile(QString path){
    nextFile = path;
}

void mplayer::play(){    
    /*! Continues playback if play is pressed while the player is paused*/
    if(paused == true){
        pause();
        return;
    }    

    currentFile = nextFile;
    QByteArray cmd = "loadfile ";

    /*! Add double quotes to avoid missinterpretation of path by the player*/
    QString path = currentFile;
    path.prepend("\"");
    path.append("\"");
    cmd.append(path);

    sendCommandToPlayer(cmd);

    /*! Continues playback if play is pressed while the player is paused*/
    paused = false;

    return;

}

void mplayer::stop(){
    sendCommandToPlayer("stop");
    currentFile = "";
}

void mplayer::ff(int frames){
    QString cmd = "seek +";
    cmd.append(QString::number(frames/100));
    sendCommandToPlayer(cmd);
}

void mplayer::rw(int frames){
    QString cmd = "seek -";
    cmd.append(QString::number(frames/100));
    sendCommandToPlayer(cmd);
}

void mplayer::pause(){
    sendCommandToPlayer("pause");
    /*! Continues playback if play is pressed while the player is paused*/
    paused = !paused;
}

void mplayer::setVolume(int value)
{    
    sendCommandToPlayer(QString("volume %1 1").arg(value));
}
