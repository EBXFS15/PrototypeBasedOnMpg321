#include "mplayer.h"
#include <QDir>



mplayer::mplayer(QObject *parent) : QObject(parent)
{
    player = new QProcess;
    connect(player, SIGNAL(readyReadStandardOutput()), this, SLOT(player_update()));
    connect(player, SIGNAL(readyReadStandardError()), this, SLOT(player_update()));
    player->start("mplayer", QStringList() << "-slave" << "-quiet" << "-input" << "nodefault-bindings" << "-noconfig" << "all"  << "-idle");
}

mplayer::~mplayer()
{
    player->write("stop\n");
    player->write("quit 0\n");
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
                //stop();
                //emit playbackPosition(100);
                //emit playbackEnded();
            }
            else{
                int position = (currentPostion / duration)*100;
                emit playbackPosition(position);
            }
        }
    }
    emit statusChanged(msg);
}


void mplayer::loadFile(QString path){
    nextFile = path;
}

void mplayer::play(){
    /**! unpause if playback is paused and play is called */
    if(paused == true){
        pause();
        return;
    }
    player->readAll();
    currentFile = nextFile;
    QByteArray cmd = "loadfile ";
    QString path = currentFile;
    path.prepend("\"");
    path.append("\"");

    cmd.append(path);
    cmd.append("\n");
    player->write(cmd);
    paused = false;
    return;

}

void mplayer::stop(){
    player->write("stop\n");
    currentFile = "";
}

void mplayer::ff(int frames){
    QByteArray cmd = "seek +";
    cmd.append(QString::number(frames/100));
    cmd.append("\n");
    player->write(cmd);
}

void mplayer::rw(int frames){
    QByteArray cmd = "seek -";
    cmd.append(QString::number(frames/100));
    cmd.append("\n");
    player->write(cmd);
}

void mplayer::pause(){
    player->write("pause\n");
    paused = !paused;
}
