#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include "kvp_keyvalueparser.h"
#include "gpio.h"
#include <QCoreApplication>
#include <QDir>


// The configuration file name
#define THE_CONFIG_FILE_NAME ".ebxfs15_musicplayer_config"
// The maximum number of supported playlist's
#define MAX_NBR_OF_PLAYLIST  (255)
#define PLAY_VOLUME 100
/*!
 * \brief Constructor
 *
 * \param parent widget
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    Key_SW1_GPIO(7),
    Key_SW2_GPIO(60),
    Key_SW3_GPIO(48),
    Key_SW4_GPIO(51),
    Key_SW5_GPIO(115),
    Led_1_GPIO(68),
    Led_2_GPIO(69),
    Led_Timeout(1000),
    Key_Timeout(180),
    myConfigFile( QDir::homePath().append(QDir::separator()).append(THE_CONFIG_FILE_NAME))
{     

    // --- Setup GPIO's for KEY's and LED's ---
    // LED 1
    gpio_export(Led_1_GPIO);
    gpio_setToOutput(Led_1_GPIO);
    // LED 2
    gpio_export(Led_2_GPIO);
    gpio_setToOutput(Led_2_GPIO);

    // Turn on AMPLIFIER
    gpio_set(Led_1_GPIO);

    // SW1
    gpio_export(Key_SW1_GPIO);
    gpio_setToInput(Key_SW1_GPIO);
    // SW2
    gpio_export(Key_SW2_GPIO);
    gpio_setToInput(Key_SW2_GPIO);
    // SW3
    gpio_export(Key_SW3_GPIO);
    gpio_setToInput(Key_SW3_GPIO);
    // SW4
    gpio_export(Key_SW4_GPIO);
    gpio_setToInput(Key_SW4_GPIO);
    // SW5
    gpio_export(Key_SW5_GPIO);
    gpio_setToInput(Key_SW5_GPIO);



    // --- Set and start LED timer ---
    QTimer *timer_LED = new QTimer(this);
    connect(timer_LED, SIGNAL(timeout()), this, SLOT(timeout_LED()));
    timer_LED->start(Led_Timeout);

    // --- Set and start KEY timer ---
    QTimer *timer_KEY = new QTimer(this);
    connect(timer_KEY, SIGNAL(timeout()), this, SLOT(timeout_KEY()));
    timer_KEY->start(Key_Timeout);

    player = new mplayer();

    connect(player,SIGNAL(statusChanged(QString)), this, SLOT(player_update(QString)));
    connect(player,SIGNAL(playbackInfo(QString)), this, SLOT(player_update(QString)));
    connect(player,SIGNAL(playbackPosition(int)), this, SLOT(on_currentPosition(int)));
    connect(player,SIGNAL(playbackStarted()), this, SLOT(on_playbackStarted()));
    connect(player,SIGNAL(playbackEnded()), this, SLOT(on_playbackEnded()));

    uartListener = new QThread();
    rfidListener * uartWorker = new rfidListener();
    uartWorker->moveToThread(uartListener);

    connect(uartListener,SIGNAL(started()),uartWorker,SLOT(process()));
    connect(uartWorker,SIGNAL(newTagDetected(QString)), this,SLOT(rfidTagDetected(QString)));
    //connect(uartWorker, SIGNAL(newTagDetected(QString)), uartListener, SLOT(quit()));
    //connect(uartWorker, SIGNAL(newTagDetected(QString)), uartWorker, SLOT(deleteLater()));
    //connect(uartListener, SIGNAL(finished()), uartListener, SLOT(deleteLater()));

    uartListener->start();    

    ui->setupUi(this);
    player->setVolume(ui->volumeSlider->value());
    ui->statusBar->showMessage("Idle");

    // --- Start PA Alimentation ---



    //gpio_burst(Led_1_GPIO);
    // --- Initialise the play lists from .config file ---
    initPlayList();        
    grabGesture(Qt::SwipeGesture);
}

/*!
 * \brief Destructor
 * \details
 * GPIOs will be unexported here.
 */
MainWindow::~MainWindow()
{
    /** Cedric: TODO: Move this to ~MainWindow()
     */    

    // --- Free GIO resources ---
    // SW1
    gpio_unexport(Key_SW1_GPIO);
    // SW2
    gpio_unexport(Key_SW2_GPIO);
    // SW3
    gpio_unexport(Key_SW3_GPIO);
    // SW4
    gpio_unexport(Key_SW4_GPIO);
    // SW5
    gpio_unexport(Key_SW5_GPIO);

    // LED1
    gpio_clear(Led_1_GPIO);
    gpio_unexport(Led_1_GPIO);
    // LED2
    gpio_clear(Led_2_GPIO);
    gpio_unexport(Led_2_GPIO);


    uartListener->terminate();
    delete uartListener;
    delete player;
    delete ui;
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
    if ((ui->FileList->currentRow()+1) < ui->FileList->count()){
        ui->FileList->setCurrentRow(ui->FileList->currentRow()+1);
        ui->FileList->itemClicked(ui->FileList->currentItem());
        return true;
    }
    else{
        return false;
    }
}


/*!
 * \brief Play next media file in the current playlist
 */
bool MainWindow::playPrevious()
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

void MainWindow::on_btn_close_pressed()
{     

}

void MainWindow::on_btn_pause_pressed()
{
    player->pause();
}

void MainWindow::on_playbackStarted(){
    playbackOnGoing(true);
}

void MainWindow::on_currentPosition(int position){
    this->ui->hBar_position->setValue(position);
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

    uartListener->quit();
    uartListener->start();
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
    char playListName[100]          = {'\0'};
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
                                          100);         // length of playListName buffer

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
    const QString playList = "Default";
    on_PlayList_activated(playList);
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
    char playListName[100]          = {'\0'};

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
                                          100);         // length of playListName buffer

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
 * \brief Get playlist from name
 *
 * \param playlist rfid string
 * \returns \p integer id of the playlist found. returns last playlist id if no playlist found
 */
uint8_t
MainWindow::get_PlayList_from_rfid(const char* inRfidP)
{
    uint8_t nbr = 0;
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "rfid";
    char rfidName[100]              = {'\0'};

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
                                          100);         // length of playListName buffer

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

/*!
 * \brief Update file list if one playlist is chosen
 *
 * \param playlist string name
 */
void
MainWindow::on_PlayList_activated(const QString &arg1)
{
    ui->FileList->clear();    

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
    char songName[100]          = {'\0'};
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
                                      100);        // length of songName buffer

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
                                          100);        // length of songName buffer

        if (status == KvpAttributeSuccess) {
            ui->FileList->addItem(songName);
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
    player->loadFile(item->text());
    player->play();
} // MainWindow::on_FileList_itemClicked

#define IR_STATE_MUTE 7
#define IR_STATE_START IR_STATE_MUTE + 1
#define IR_STATE_PREPARE_SET_IN    IR_STATE_MUTE - 1
#define IR_STATE_SET_AM    IR_STATE_PREPARE_SET_IN - 1
#define IR_STATE_SET_AUX   IR_STATE_SET_AM - 1
#define IR_STATE_UNMUTE IR_STATE_SET_AUX - 4

void
MainWindow::timeout_LED(void)
{
    static unsigned short x = 0;
    static unsigned short irTimeout = 8;
    QByteArray data;
    QFile file("/dev/jvc-remote");
    x++;
    if (x % 2) {
      gpio_set(Led_2_GPIO);
    } else {
      gpio_clear(Led_2_GPIO);
    }
    if(irTimeout == IR_STATE_MUTE)
    {
        int i = 0;

        //Mute
        data.clear();
        data.append(0x47);
        data.append(0x05);
        for(i = 0 ; i< 20;i++)
        {
            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
        }
        irTimeout--;
    }
    if(irTimeout == IR_STATE_PREPARE_SET_IN)
    {
        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        irTimeout--;
    }
    if(irTimeout == IR_STATE_SET_AM)
    {
        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        irTimeout--;
    }

    if(irTimeout == IR_STATE_SET_AUX)
    {
        int i = 0;

        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();

        irTimeout--;
    }
    if(irTimeout == IR_STATE_UNMUTE)
    {
        int i = 0;
        // UnMute
        data.clear();
        data.append(0x47);
        data.append(0x04);
        for(i = 0 ; i< 16;i++)
        {
            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
        }

        irTimeout--;
    }
    if(irTimeout > IR_STATE_UNMUTE)
    {
        irTimeout--;
    }


} // MainWindow::timeout_LED

void
MainWindow::timeout_KEY(void)
{
    // --- Handle KEY SW1 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw1Pressed = false;

    int key = 1;
    gpio_get(Key_SW1_GPIO, &key);
    if ((0 == key) && (false == isSw1Pressed)) {
        // SW1 pressed
        ui->btn_stop->click();
        isSw1Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw1Pressed = false;
        }
    }

    // --- Handle KEY SW2 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw2Pressed = false;

    key = 1;
    gpio_get(Key_SW2_GPIO, &key);
    if ((0 == key) && (false == isSw2Pressed)) {
        // SW1 pressed
        ui->btn_pause->click();
        isSw2Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw2Pressed = false;
        }
    }

    // --- Handle KEY SW3 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw3Pressed = false;

    key = 1;
    gpio_get(Key_SW3_GPIO, &key);
    if ((0 == key) && (false == isSw3Pressed)) {
        // SW1 pressed
        ui->btn_ff->click();
        isSw3Pressed = false; //true; // do NOT set to true to allow fast ff
    } else {
        // Wait for key released
        if (1 == key) {
          isSw3Pressed = false;
        }
    }

    // --- Handle KEY SW4 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw4Pressed = false;

    key = 1;
    gpio_get(Key_SW4_GPIO, &key);
    if ((0 == key) && (false == isSw4Pressed)) {
        // SW1 pressed
        ui->btn_RW->click();
        isSw4Pressed = false; //true; // do NOT set to true to allow fast rew
    } else {
        // Wait for key released
        if (1 == key) {
          isSw4Pressed = false;
        }
    }

    // --- Handle KEY SW5 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw5Pressed = false;

    key = 1;
    gpio_get(Key_SW5_GPIO, &key);
    if ((0 == key) && (false == isSw5Pressed)) {
        // SW1 pressed
        ui->btn_play->pressed();
        isSw5Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw5Pressed = false;
        }
    }
} // MainWindow::timeout_KEY

void
MainWindow::on_volumeSlider_valueChanged(int value)
{
    if(value<=ui->volumeSlider->maximum() &&
       value>=ui->volumeSlider->minimum())
    {
        player->setVolume(value);
    }
    else
    {
        ui->statusBar->showMessage("Error: wrong volume.");
    }
}



void MainWindow::on_PlayList_activated(int index)
{

}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool MainWindow::gestureEvent(QGestureEvent *event)
{
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
    QByteArray data;
    QFile file("/dev/jvc-remote");
    int i;
    //Mute
    data.clear();
    data.append(0x47);
    data.append(0x05);
    for(i = 0 ; i< 25;i++)
    {
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
    }
}
