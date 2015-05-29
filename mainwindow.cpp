#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include "kvp_keyvalueparser.h"
#include "gpio.h"


// TODO:
// - Extend MainWindow::timeout_KEY() to handle the other key's (SW2-5).
//   Use the same timer for every key or a seperate timer for each key?
//

// IMPROVEMENT:
// - The directory name where the .config file is stored could set as an application argument.
#define THE_CONFIG_FILE_PATH "/media/mp3"
#define THE_CONFIG_FILE_NAME ".config"



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
    Key_Timeout(100)
{
    // --- Setup GPIO's for KEY's and LED's ---
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

    // LED 1
    gpio_export(Led_1_GPIO);
    gpio_setToOutput(Led_1_GPIO);
    // LED 2
    gpio_export(Led_2_GPIO);
    gpio_setToOutput(Led_2_GPIO);

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

    ui->setupUi(this);

    ui->statusBar->showMessage("Idle");

    // --- Initialise the play lists from .config file ---
    initPlayList();
}

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

    delete player;
    delete ui;
}

void MainWindow::player_update(QString newStatus)
{
    ui->textEdit->append(newStatus);
}

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
    player->ff(100);
}

void MainWindow::on_btn_RW_pressed()
{
    player->rw(100);
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

void
MainWindow::initPlayList(void)
{
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "name";
    char playListName[100]          = {'\0'};
    uint32_t fileId                 = 0;

    int ret = kvp_fileOpen(THE_CONFIG_FILE_PATH"/"THE_CONFIG_FILE_NAME, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        // TODO: What error handling do we provide....a status line?
        /** Cedric: I would suggest to pop up a messagebox and to close the application.
         *  Reason: what can the user do through our GUI to solve the problem
         * */
        return;
    }

    KvpAttributeState status = KvpAttributeSuccess;
    for (uint8_t nbr = 1; status == KvpAttributeSuccess; nbr++) {


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
        // TODO: What error handling do we provide....a status line?       
        /** Cedric question: What happens if the file was not closed correctyl. Has it any impact on the behaviour of the player?
         */
    }

    // Show the files from the default play list
    const QString playList = "Default";
    on_PlayList_activated(playList);
} // MainWindow::initPlayList

uint8_t
MainWindow::get_PlayList_from_name(const char* inNameP)
{
    uint8_t nbr = 0;
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "name";
    char playListName[100]          = {'\0'};

    uint32_t fileId = 0;
    int ret = kvp_fileOpen(THE_CONFIG_FILE_PATH"/"THE_CONFIG_FILE_NAME, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        // TODO: What error handling do we provide....a status line?
        /** Cedric: I would suggest to pop up a messagebox and to close the application.
         *  Reason: what can the user do through our GUI to solve the problem
         * */
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
        // TODO: What error handling do we provide....a status line?
        /** Cedric question: What happens if the file was not closed correctyl. Has it any impact on the behaviour of the player?
         */
    }

    return nbr;
} // MainWindow::get_PlayList_from_name

uint8_t
MainWindow::get_PlayList_from_rfid(const char* inRfidP)
{
    uint8_t nbr = 0;
    static const char PlayListTag[] = "PlayList";
    char sectionName[30]            = {'\0'};
    char key[]                      = "rfid";
    char rfidName[100]              = {'\0'};

    uint32_t fileId = 0;
    int ret = kvp_fileOpen(THE_CONFIG_FILE_PATH"/"THE_CONFIG_FILE_NAME, &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        // TODO: What error handling do we provide....a status line?
        /** Cedric: As above
         */
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
        // TODO: What error handling do we provide....a status line?
        /** Cedric: As above
         */
    }

    return nbr;
} // MainWindow::get_PlayList_from_rfid

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
    /** Cedric: Why do we have here a magic path? Is there any reason for this?
     */
    ret = kvp_fileOpen("/media/mp3/.config", &fileId, false);
    if (0 != ret) {
        // Could not open the .config file.
        // TODO: What error handling do we provide....a status line?
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

    QImage imageObject;
    imageObject.load(imageName);
    ui->mediaImage->setPixmap(QPixmap::fromImage(imageObject));

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
    }
} // MainWindow::on_PlayList_activated

void
MainWindow::on_FileList_itemClicked(QListWidgetItem *item)
{
    player->loadFile(item->text());
    player->play();

} // MainWindow::on_FileList_itemClicked

void
MainWindow::timeout_LED(void)
{
    static unsigned short x = 0;
    x++;
    if (x % 2) {
      gpio_set(Led_1_GPIO);
      gpio_clear(Led_2_GPIO);
    } else {
      gpio_clear(Led_1_GPIO);
      gpio_set(Led_2_GPIO);
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
        ui->btn_close->click();
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
