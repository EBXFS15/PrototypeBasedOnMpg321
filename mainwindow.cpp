#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include "kvp_keyvalueparser.h"
#include <QCoreApplication>
#include <QDir>
#include <QEvent>
#include <QMetaEnum>


// The configuration file name
#define THE_CONFIG_FILE_NAME ".ebxfs15_musicplayer_config"
// The maximum number of supported playlist's
#define MAX_NBR_OF_PLAYLIST  (255)
#define PLAY_VOLUME 100
#define FILENAME_BUFFER 180
/*!
 * \brief Constructor
 *
 * \param parent widget
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), 
    ui(new Ui::MainWindow),
    Led_Timeout(1000),
    Key_Timeout(180),
    myConfigFile( QDir::homePath().append(QDir::separator()).append(THE_CONFIG_FILE_NAME))
{     
    hwHal = new hal();

    player = new mplayer();

    connect(player,SIGNAL(statusChanged(QString)), this, SLOT(player_update(QString)));
    connect(player,SIGNAL(playbackInfo(QString)), this, SLOT(player_update(QString)));
    connect(player,SIGNAL(playbackPosition(int)), this, SLOT(on_currentPosition(int)));
    connect(player,SIGNAL(playbackStarted()), this, SLOT(on_playbackStarted()));
    connect(player,SIGNAL(playbackEnded()), this, SLOT(on_playbackEnded()));
    connect(hwHal, SIGNAL(ready(void)), this, SLOT(hardwareReady(void)));

    connect(hwHal, SIGNAL(key_SW1_pressed(void)), this, SLOT(on_btn_play_pressed()));
    connect(hwHal, SIGNAL(key_SW2_pressed(void)), this, SLOT(on_btn_pause_pressed()));
    connect(hwHal, SIGNAL(key_SW3_pressed(void)), this, SLOT(on_btn_RW_pressed()));
    connect(hwHal, SIGNAL(key_SW4_pressed(void)), this, SLOT(on_btn_ff_pressed()));
    connect(hwHal, SIGNAL(key_SW5_pressed(void)), this, SLOT(on_btn_stop_pressed()));

    connect(hwHal, SIGNAL(newTagDetected(QString)), this,SLOT(rfidTagDetected(QString)));

    connect(&continueToPlay,SIGNAL(startPlayback(QString,int,int)),this,SLOT(startPlayback(QString,int,int)));

    tempPlaylist = -1;
    ui->setupUi(this);

    // --- Initialise the play lists from .config file ---
    initPlayList();        

}

void MainWindow::hardwareReady()
{
    player->setVolume(10);
    ui->statusBar->showMessage("Idle");
}


/*!
 * \brief Destructor
 * \details
 */
MainWindow::~MainWindow()
{
    delete hwHal;    
    delete player;
    delete ui;
}

void MainWindow::startPlayback(QString playlist, int track, int position)
{
    //ui->btn_stop();
    ui->PlayList->setCurrentText(playlist);
    on_PlayList_activated(playlist);
    ui->statusBar->showMessage(playlist.prepend("Loading last plaid playlist: "));

    if (track < ui->FileList->count()){
        ui->FileList->setCurrentRow(track);
        ui->FileList->itemClicked(ui->FileList->currentItem());
        emit player->pos(position);
        // Add here the update for the current track
    }
}

/*!
 * \brief Update player status bar
 *
 * \param string of the new status
 */
void MainWindow::player_update(QString newStatus)
{
    ui->statusBar->showMessage(newStatus);
}

/*!
 * \brief Play next media file in the current playlist
 */
bool MainWindow::playNext()
{

    if(ui->btn_singleRepeat->isChecked())
    {
        ui->FileList->itemClicked(ui->FileList->currentItem());
        return true;
    }
    else
    {
        if ((ui->FileList->currentRow()+1) < ui->FileList->count()){
            ui->FileList->setCurrentRow(ui->FileList->currentRow()+1);
            ui->FileList->itemClicked(ui->FileList->currentItem());
            // Add here the update for the current track
            return true;
        }
        else{
            if(ui->btn_cd->isChecked()){
                ui->FileList->setCurrentRow(0);
                ui->FileList->itemClicked(ui->FileList->currentItem());
                return true;
            }
            else{
                return false;
            }
        }
    }
}


/*!
 * \brief Play next media file in the current playlist
 */
bool MainWindow::playPrevious()
{
    if(ui->btn_singleRepeat->isChecked())
    {
        ui->FileList->itemClicked(ui->FileList->currentItem());
        return true;
    }
    else
    {
        if ((ui->FileList->currentRow()-1) >= 0){
            ui->FileList->setCurrentRow(ui->FileList->currentRow()-1);
            ui->FileList->itemClicked(ui->FileList->currentItem());
            return true;
        }
        else{
            return false;
        }
    }
}

void MainWindow::on_btn_play_pressed()
{
    if(ui->FileList->currentRow() < 0){
        playNext();        
    }
    else{
        player->play();                
    }
}

void MainWindow::on_btn_stop_pressed()
{
    playbackOnGoing(false);
    player->stop();
}

void MainWindow::on_btn_ff_pressed()
{
    playNext();
}

void MainWindow::on_btn_RW_pressed()
{
    playPrevious();
}


void MainWindow::on_btn_pause_pressed()
{
    player->pause();
}


void MainWindow::on_btn_sleep_pressed()
{
    player->pause();
    // store currently selected playlist
    continueToPlay.setCurrentPlaylist(ui->PlayList->currentText());
    continueToPlay.setCurrentTrack(ui->FileList->currentRow());
    continueToPlay.setPlaybackPosition(this->ui->hBar_position->value()/10);
    continueToPlay.store();
}


void MainWindow::on_playbackStarted(){
    playbackOnGoing(true);
}

void MainWindow::on_currentPosition(int position){
    //save position
    this->ui->hBar_position->setValue(position);
    //continueToPlay.setPlaybackPosition(position/10);
}

void MainWindow::on_playbackEnded(){
    playbackOnGoing(false);
    playNext();
}

void MainWindow::playbackOnGoing(bool value){
    ui->btn_pause->setEnabled(value);
    ui->btn_stop->setEnabled(value);
    ui->btn_ff->setEnabled(value);
    ui->btn_RW->setEnabled(value);

    if(value)
    {
        ui->statusBar->showMessage("Playing");
    }
    else
    {
        ui->statusBar->showMessage("Idle");
    }
}

void MainWindow::rfidTagDetected(QString tagId){
    int playListNumber = get_PlayList_from_rfid(tagId.toLocal8Bit());
    if (playListNumber <= ui->PlayList->count()){
        playbackOnGoing(false);
        player->stop();
        ui->PlayList->setCurrentIndex(playListNumber-1);
        on_PlayList_activated(ui->PlayList->currentText());
        ui->statusBar->showMessage(tagId.prepend("Known TAG-Discovered: "));
        playNext();
    }
    else{
        ui->statusBar->showMessage(tagId.prepend("Unknown TAG-Discovered: "));
    }
}

/*!
 * \brief Init playlist
 * \details
 * Playlist are parsed from a config file
 */
void
MainWindow::initPlayList(void)
{
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "name";
    char playListName[FILENAME_BUFFER]          = {'\0'};
    uint32_t fileId                 = 0;

    const char* configFileP = myConfigFile.toStdString().c_str();
    int ret = kvp_fileOpen(configFileP, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        showMessageBoxAndClose("Cannot open config file! The application will be closed.\n" + myConfigFile);
        this->hide();
        this->deleteLater();
        return;
    }

    KvpAttributeState status = KvpAttributeSuccess;
    for (uint16_t nbr = 1; nbr <= MAX_NBR_OF_PLAYLIST; nbr++) {


        sectionName[0] = '\0';
        int ret = snprintf(sectionName, 30, "%s%d", PlayListTag, nbr);
        if (ret >= 30) {
            // snprintf has truncated the name due to to small size of sectionName.
            // This can never happen as the [Section Name] is part of the application.
            // But we handle the error and skip this play list and continue with the next one,
            continue;
        }

        status = kvp_getAttributeByFileId(fileId,
                                          '=',          // delimiter
                                          sectionName,  // section
                                          key,          // key
                                          playListName, // value
                                          FILENAME_BUFFER);         // length of playListName buffer

        if (status == KvpAttributeSuccess) {
            ui->PlayList->addItem(playListName);

        }
    } // for (uint8_t nbr = 1; status == KvpAttributeSuccess; nbr++;)

    bool ok = kvp_fileClose(fileId);
    if (false == ok) {
        // Could not close the .config file.
        showMessageBoxAndClose("Cannot close config file! The application will be closed.");
    }

    // Show the files from the default play list
    //const QString playList = "Default";
    //on_PlayList_activated(playList);
} // MainWindow::initPlayList

/*!
 * \brief Get playlist from name
 *
 * \param playlist name string name
 * \returns \p id integer of the playlist found. returns last playlist id if no playlist found
 */
uint8_t
MainWindow::get_PlayList_from_name(const char* inNameP)
{
    uint8_t nbr = 0;
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "name";
    char playListName[FILENAME_BUFFER ]          = {'\0'};

    uint32_t fileId = 0;
    const char* configFileP = myConfigFile.toStdString().c_str();
    int ret = kvp_fileOpen(configFileP, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        showMessageBoxAndClose("Cannot open config file! The application will be closed.");
        return 255;
    }

    KvpAttributeState status = KvpAttributeSuccess;
    for (nbr = 1; status == KvpAttributeSuccess; nbr++) {
        sectionName[0] = '\0';
        int ret = snprintf(sectionName, 30, "%s%d", PlayListTag, nbr);
        if (ret >= 30) {
            // snprintf has truncated the name due to to small size of sectionName.
            // This can never happen as the [Section Name] is part of the application.
            // But we handle the error and skip this play list and continue with the next one,
            continue;
        }

        status = kvp_getAttributeByFileId(fileId,
                                          '=',          // delimiter
                                          sectionName,  // section
                                          key,          // key
                                          playListName, // value
                                          FILENAME_BUFFER );         // length of playListName buffer

        if (status == KvpAttributeSuccess) {
            int ret = strcmp(inNameP, playListName);
            if (0 == ret) {
                // found
                break;
            }
        }
    } // for (uint8_t nbr = 1; status == KvpAttributeSuccess; nbr++;)


    bool ok = kvp_fileClose(fileId);
    if (false == ok) {
        // Could not close the .config file.
        showMessageBoxAndClose("Cannot close config file! The application will be closed.");
    }

    return nbr;
} // MainWindow::get_PlayList_from_name

/*!
 * \brief Get playlist from rfid-tag
 *
 * \param inRfidP rfid string
 * \returns \p integer id of the playlist found. returns last playlist id if no playlist found
 */
uint8_t
MainWindow::get_PlayList_from_rfid(const char* inRfidP)
{
    uint8_t nbr = 0;
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "rfid";
    char rfidName[FILENAME_BUFFER ]              = {'\0'};

    uint32_t fileId = 0;
    const char* configFileP = myConfigFile.toStdString().c_str();
    int ret = kvp_fileOpen(configFileP, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        showMessageBoxAndClose("Cannot open config file! The application will be closed.");
        return 255;
    }

    KvpAttributeState status = KvpAttributeSuccess;
    for (nbr = 1; status == KvpAttributeSuccess; nbr++) {


        sectionName[0] = '\0';
        int ret = snprintf(sectionName, 30, "%s%d", PlayListTag, nbr);
        if (ret >= 30) {
            // snprintf has truncated the name due to to small size of sectionName.
            // This can never happen as the [Section Name] is part of the application.
            // But we handle the error and skip this play list and continue with the next one,
            continue;
        }

        status = kvp_getAttributeByFileId(fileId,
                                          '=',          // delimiter
                                          sectionName,  // section
                                          key,          // key
                                          rfidName,     // value
                                          FILENAME_BUFFER );         // length of playListName buffer

        if (status == KvpAttributeSuccess) {            
            rfidName[14] = '\0';
            int ret = strcmp(inRfidP, rfidName);
            if (0 == ret) {
                // found
                break;
            }
        }
    } // for (uint8_t nbr = 1; status == KvpAttributeSuccess; nbr++;)


    bool ok = kvp_fileClose(fileId);
    if (false == ok) {
        // Could not close the .config file.
        showMessageBoxAndClose("Cannot close config file! The application will be closed.");
    }

    return nbr;
} // MainWindow::get_PlayList_from_rfid

void MainWindow::on_btn_previousAlbum_pressed()
{
    int listItem = ui->PlayList->currentIndex()-1;
    if (listItem < 0)
    {
        listItem = ui->PlayList->count()-1;
    }
    show_Playlist(listItem);
}

void MainWindow::on_btn_nextAlbum_pressed()
{
    int listItem = ui->PlayList->currentIndex()+1;
    if (listItem >= ui->PlayList->count())
    {
        listItem = 0;
    }
    show_Playlist(listItem);
}

void MainWindow::on_btn_openAlbum_pressed()
{
    if(tempPlaylist>=0){
        on_PlayList_activated(ui->PlayList->itemText(tempPlaylist));
        on_btn_play_pressed();
    }
}

void MainWindow::on_btn_wakeup_pressed()
{
    // Load music that was played before shut-down
    continueToPlay.load();
}

void MainWindow::show_Playlist(int playListItem)
{
    // copy the name of the playList for further treatment
    const char* tmpP    = ui->PlayList->itemText(playListItem).toStdString().c_str();
    size_t length       = (strlen(tmpP) + 1); // +1 for zero termination
    char* playListNameP = (char*)malloc(length);
    (void)strncpy(playListNameP, tmpP, length);

    // get the play list number
    uint8_t nbr = MainWindow::get_PlayList_from_name(playListNameP);

    // free the buffer, we do not need it anymore
    free(playListNameP);

    //static const char PathTag[] = "path";
    char sectionName[30]        = {'\0'};
    //char key[20]                = {'\0'};
    char imageKey[]             = "image";
    //char songName[FILENAME_BUFFER ]          = {'\0'};
    char imageName[256]         = {'\0'};

    int ret = snprintf(sectionName, 30, "%s%d", "PlayList", nbr);
    if (ret >= 30) {
       // snprintf has truncated the name due to to small size of sectionName.
       // This can never happen as the [Section Name] is part of the application.
       // But we handle the error and return.
       return;
    }


    uint32_t fileId = 0;
    const char* configFileP = myConfigFile.toStdString().c_str();
    ret = kvp_fileOpen(configFileP, &fileId, false);
    if (0 != ret) {
       // Could not open the .config file.
       // TODO: What error handling do we provide....a status line?
       showMessageBoxAndClose("Cannot open config file! The application will be closed.");
       return;
    }

    KvpAttributeState status = KvpAttributeSuccess;
    status = KvpAttributeSuccess;

    // get image path
    status = kvp_getAttributeByFileId(fileId,
                                     '=',         // delimiter
                                     sectionName, // section
                                     imageKey,    // key
                                     imageName,   // value
                                     FILENAME_BUFFER );        // length of songName buffer

    if (status == KvpAttributeSuccess) {
        loadCover(imageName);
        ui->PlayList->setCurrentText(ui->PlayList->itemText(playListItem));
        tempPlaylist = playListItem;
    }
}

//void MainWindow::load_Playlist(const char* inNameP)
//{

//}

/*!
 * \brief Update file list if one playlist is chosen
 *
 * \param playlist string name
 */
void
MainWindow::on_PlayList_activated(const QString &arg1)
{
    currentPlaylist = ui->PlayList->currentIndex();

    ui->FileList->clear();
    currentPlayList.clear();

     // copy the name of the playList for further treatment
    const char* tmpP    = arg1.toStdString().c_str();
    size_t length       = (strlen(tmpP) + 1); // +1 for zero termination
    char* playListNameP = (char*)malloc(length);
    (void)strncpy(playListNameP, tmpP, length);

    // get the play list number
    uint8_t nbr = MainWindow::get_PlayList_from_name(playListNameP);

    // free the buffer, we do not need it anymore
    free(playListNameP);

    static const char PathTag[] = "path";
    char sectionName[30]        = {'\0'};
    char key[20]                = {'\0'};
    char imageKey[]             = "image";
    char songName[FILENAME_BUFFER ]          = {'\0'};
    char imageName[256]         = {'\0'};

    int ret = snprintf(sectionName, 30, "%s%d", "PlayList", nbr);
    if (ret >= 30) {
        // snprintf has truncated the name due to to small size of sectionName.
        // This can never happen as the [Section Name] is part of the application.
        // But we handle the error and return.
        return;
    }


    uint32_t fileId = 0;
    const char* configFileP = myConfigFile.toStdString().c_str();
    ret = kvp_fileOpen(configFileP, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        // TODO: What error handling do we provide....a status line?
        showMessageBoxAndClose("Cannot open config file! The application will be closed.");
        return;
    }

    KvpAttributeState status = KvpAttributeSuccess;

    // get image path
    status = kvp_getAttributeByFileId(fileId,
                                      '=',         // delimiter
                                      sectionName, // section
                                      imageKey,    // key
                                      imageName,   // value
                                      FILENAME_BUFFER );        // length of songName buffer

    loadCover(imageName);

    // set status to allow add files to the file list even the above image could not be found
    status = KvpAttributeSuccess;
    for (uint8_t pathNbr = 1; status == KvpAttributeSuccess; pathNbr++) {

        ret = snprintf(key, 30, "%s%d", PathTag, pathNbr);
        if (ret >= 30) {
            // snprintf has truncated the name due to to small size of pathName.
            // This can never happen as the path tag is part of the application.
            // But we handle the error and skip this path.
            continue;
        }

        status = kvp_getAttributeByFileId(fileId,
                                          '=',         // delimiter
                                          sectionName, // section
                                          key,         // key
                                          songName,    // value
                                          FILENAME_BUFFER);        // length of songName buffer

        if (status == KvpAttributeSuccess) {
            QString filename = songName;

            ui->FileList->addItem(filename.split("/").back());

            currentPlayList.push_back(songName);
        }

    } // for (uint8_t nbr = 1; status == KvpAttributeSuccess; nbr++;)


    bool ok = kvp_fileClose(fileId);
    if (false == ok) {
        // Could not close the .config file.
        // TODO: What error handling do we provide....a status line?
        /** Cedric: As above
         */
        showMessageBoxAndClose("Cannot close config file! The application will be closed.");
    }
} // MainWindow::on_PlayList_activated

void
MainWindow::showMessageBoxAndClose(QString msg)
{
   QMessageBox box;
   box.setWindowTitle("Critical Error");
   box.setText(msg);
   box.setStandardButtons(QMessageBox::Ok);
   box.exec();
   this->close();
}

void MainWindow::loadCoverThroughFfmpeg(QString path){
    QProcess ffmpegProcess;
    QStringList par;
    par << "-i" << path << "cover.jpg";
    ffmpegProcess.start("ffmpeg",par);
    ffmpegProcess.closeWriteChannel();
    ffmpegProcess.waitForFinished();
    QImage imageObject;
    loadCover("cover.jpg");
}

void MainWindow::loadCover(QString path){
    QFileInfo file(path);
    if (file.exists()){
        QImage imageObject;
        imageObject.load(path);
        QImage scaled = imageObject.scaled(ui->mediaImage->minimumWidth(),ui->mediaImage->minimumHeight());
        ui->mediaImage->setPixmap(QPixmap::fromImage(scaled));
    }
    else{                
        QImage imageObject(":/icons/Resources/icons/logo.png");
        QImage scaled = imageObject.scaled(ui->mediaImage->minimumWidth(),ui->mediaImage->minimumHeight());
        ui->mediaImage->setPixmap(QPixmap::fromImage(scaled));
    }
}

void
MainWindow::on_FileList_itemClicked(QListWidgetItem *item)
{
    int itemIndex = ui->FileList->row(item);

    player->loadFile(currentPlayList.at(itemIndex));
    player->play();
} // MainWindow::on_FileList_itemClicked





//void MainWindow::on_PlayList_activated(int index)
//{

//}

bool MainWindow::event(QEvent *event)
{
//    static int eventEnumIndex = QEvent::staticMetaObject.indexOfEnumerator("Type");
//    if(event->type() != QEvent::UpdateRequest){
//        qDebug() << "gestureEvent():" << QEvent::staticMetaObject
//                .enumerator(eventEnumIndex).valueToKey(event->type());
//    }
    if (event->type() == QEvent::Gesture)
    {

        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

bool MainWindow::gestureEvent(QGestureEvent *event)
{
    event->accept();
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    return true;
}

void MainWindow::swipeTriggered(QSwipeGesture *gesture)
{
    if (gesture->state() == Qt::GestureFinished) {
        if (gesture->horizontalDirection() == QSwipeGesture::Left
            || gesture->verticalDirection() == QSwipeGesture::Up) {

                playNext();
            //goPrevImage();
        } else {
                playPrevious();
            //goNextImage();
        }
        update();
    }
}
void MainWindow::on_btn_cd_pressed()
{    
    //continueToPlay.setPlaybackPosition(ui->hBar_position->value()/1);
}
