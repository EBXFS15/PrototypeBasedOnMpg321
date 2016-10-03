#include "mplayer.h"
#include <QDir>
#include <QFileInfo>

static bool fileExists(QString path)
{
    QFileInfo checkFile(path);
    if(checkFile.exists() && checkFile.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief Constructor
 *
 * \param parent object
 */
mplayer::mplayer(QObject *parent) : QObject(parent)
{
    player = new QProcess;
    connect(player, SIGNAL(readyReadStandardOutput()), this, SLOT(player_update()));
    connect(player, SIGNAL(readyReadStandardError()), this, SLOT(player_update()));
    seeking = 0;
    paused = false;
    startProcess();
}

/*!
 * \brief start mplayer process
 *
 * \param windows id to redirect the video stream
 */
void mplayer::startProcess(int winId)
{
/*!
  * The following options configure the mplayer to issue at the end of each file a clear signal "EOF code: 1"
  * really-quiet=1
  * msglevel=statusline=6
  * msglevel=global=6
  */
QStringList arguments;
arguments << "-slave";
#ifdef NO_VIDEO
    arguments << "-novideo";
#else
    arguments << "-vf" << "expand=1024:768";
    arguments << "-vo" << "fbdev";
#endif
if (winId != 0){
    arguments << "-wid" << QString(winId);
}
arguments << "-input" << "nodefault-bindings";
arguments << "-really-quiet" << "1";
arguments << "-msglevel" << "statusline=6";
arguments << "-msglevel" << "global=6";
arguments << "-idle";
player->start("mplayer", arguments);
}

/*!
 * \brief Destructor
 * \details
 * This function will ask the MPlayer process to quit and block until it has really
 * finished.
 */
mplayer::~mplayer()
{
    sendCommandToPlayer("stop");
    sendCommandToPlayer("quit");
    player->closeWriteChannel();
    player->waitForFinished();
    delete player;
}

/*!
 * \brief update mplayer status
 * \details
 * Since MPlayer is run in slave mode, it reads commands from the standard
 * input. Both standard and error output are parsed  here.
 *
 * For a complete list of commands for MPlayer's slave mode, see
 * http://www.mplayerhq.hu/DOCS/tech/slave.txt .
 */
void mplayer::player_update()
{
    QStringList lines = QString::fromLocal8Bit(player->readAll()).split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < lines.count(); i++) {
        lines[i].remove("\r");
        parseLine(lines[i]);
    }
}


/*!
 * \brief parse mplayer status (one line at a time)
 *
 * \param string of the current line to parse
 */
void mplayer::parseLine(const QString &line)
{
    if (line.startsWith("File not found: ")) {
        emit statusChanged("Error: FileNotFound");
    } else if (line.contains("EOF code: 1")) {
        emit playbackPosition(1000);
        emit playbackEnded();
    } else if (line.startsWith("A:") || line.startsWith("V:")) {
        parsePosition(line);
    } else if (line.startsWith("ANS_pause=")) {
        if(line=="ANS_pause=yes")
        {
            if(paused)
            {
                emit statusChanged("Paused");
            }
            else
            {
                emit statusChanged("Error: cannot pause");
            }
        }
        else if(line=="ANS_pause=no")
        {
            if(!paused)
            {
                emit statusChanged("Playing");
            }
            else
            {
                emit statusChanged("Error: cannot start again");
            }
        }
    } else if (line.startsWith("ANS_META_ARTIST=")) {

        m_mediaInfo.artist=line.mid(QString("ANS_META_ARTIST=").size());
    } else if (line.startsWith("ANS_META_ALBUM=")) {

        m_mediaInfo.album=line.mid(QString("ANS_META_ALBUM=").size());
    } else if (line.startsWith("ANS_META_TITLE=")) {

        m_mediaInfo.title=line.mid(QString("ANS_META_TITLE=").size());
    }
    //emit statusChanged(line);
}

/*!
 * \brief parse media position
 *
 * \param string of the current line to parse
 */
void mplayer::parsePosition(const QString &line)
{
    QString playBackInfo = line.mid(line.lastIndexOf("A:"));
    if(playBackInfo.startsWith("A:")){// && playBackInfo.endsWith("\r")){
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
            int position = (currentPostion / duration)*1000;
            emit playbackPosition(position);
            if (seeking >= 0){
                seeking--;
            }
        }
    }
}

/*!
 * \brief send command to mplayer slave
 * \details
 * Adds a line-feed to the QByteArray in order to trigger the player to start interpretation of cmd
 *
 * \param command string
 */
void mplayer::sendCommandToPlayer(QString cmd){
    cmd.append("\n");
    player->write(cmd.toLocal8Bit());
}

/*!
 * \brief load media file
 * \details
 * defines the file to play when the play function is called
 *
 * \param path string to the media file
 */
void mplayer::loadFile(QString path){
    nextFile = path;
}

/*!
 * \brief play media file
 * \details
 * Play media file and send command to get meta data from media file played
 */
void mplayer::play(){    
    /*! Continues playback if play is pressed while the player is paused*/
    if(paused == true){
        pause();
        return;
    }    

    currentFile = nextFile;
    // verify that file exists
    if(!fileExists(currentFile))
    {
        emit statusChanged("Error: file does not exist");
        emit playbackEnded();
        return;
    }

    QByteArray cmd = "loadfile ";

    /*! Add double quotes to avoid missinterpretation of path by the player*/
    QString path = currentFile;
    path.prepend("\"");
    path.append("\"");
    cmd.append(path);

    sendCommandToPlayer(cmd);

    /*! Continues playback if play is pressed while the player is paused*/
    paused = false;
    sendCommandToPlayer("get_meta_artist");
    sendCommandToPlayer("get_meta_album");
    sendCommandToPlayer("get_meta_title");
    emit playbackStarted();
    return;
}

/*!
 * \brief stop media file
 */
void mplayer::stop(){
    sendCommandToPlayer("stop");
    currentFile = "";
}

/*!
 * \brief fastforward the media file
 *
 * \param number of frames to seek
 */
void mplayer::ff(int frames){
    if (seeking<0){
        QString cmd = "seek +";
        cmd.append(QString::number(frames/100));
        sendCommandToPlayer(cmd);
        seeking = 150;
    }
}

/*!
 * \brief go to position
 *
 * \param postion in percent of whole track len
 */
void mplayer::pos(int percentage){
    QString cmd = "seek ";
    cmd.append(QString::number(percentage));
    cmd.append(" 1");
    sendCommandToPlayer(cmd);
    seeking = 150;
}

/*!
 * \brief rewind the media file
 *
 * \param number of frames to seek
 */
void mplayer::rw(int frames){
    if (seeking<0){
        QString cmd = "seek -";
        cmd.append(QString::number(frames/100));
        sendCommandToPlayer(cmd);
        seeking = 150;
    }
}

/*!
 * \brief pause the media file
 */
void mplayer::pause(){
    sendCommandToPlayer("pause");
    /*! Continues playback if play is pressed while the player is paused*/
    paused = !paused;
    sendCommandToPlayer("pausing_keep_force get_property pause");
}

/*!
 * \brief set sound volume
 *
 * \param volume value in percent
 */
void mplayer::setVolume(int value)
{    
    sendCommandToPlayer(QString("volume %1 1").arg(value));
}

/* Documentation follows */

/*!
 * \class mplayer
 * \brief A Qt object for embedding MPlayer
 * \details
 *
 * \section Overview
 *
 * \subsection comm MPlayer communication
 *
 * If you want to communicate with MPlayer through its
 * <a href="http://www.mplayerhq.hu/DOCS/tech/slave.txt">slave mode protocol</a>,
 * you can use the sendCommandToPlayer() slot. If MPlayer writes to its standard output
 * or standard error channel, the signal player_update() will be emitted.
 *
*/
