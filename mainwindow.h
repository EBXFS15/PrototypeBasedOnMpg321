#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSwipeGesture>
#include <QGestureEvent>
#include <stdint.h>
#include "mplayer.h"
#include "hal.h"
#include "continuetoplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    mplayer * player;
    hal * hwHal;
    ContinueToPlay continueToPlay;
    int tempPlaylist;
    int currentPlaylist;
    QList<QString> currentPlayList;
    ~MainWindow();
protected:
    bool event(QEvent *event);

private slots:
    void on_btn_play_pressed();

    void on_btn_stop_pressed();

    void on_btn_ff_pressed();

    void on_btn_RW_pressed();

    //void on_btn_close_pressed();

    void on_btn_pause_pressed();

    void on_playbackStarted();

    void on_currentPosition(int position);

    void on_playbackEnded();

    // Is called for a selection in the play list.
    // The method shows the file list for the play list.
    void on_PlayList_activated(const QString &arg1);

    void player_update(QString newStatus);

    // Retrieve all play lists from the configuration
    // file an add the names to play list GUI element.
    void initPlayList(void);

    // Get the play list number from the given name.
    uint8_t get_PlayList_from_name(const char* inNameP);

    // Get the play list number from the given RFID number.
    uint8_t get_PlayList_from_rfid(const char* inRfidP);

    // Play the selected file from the file list.
    void on_FileList_itemClicked(QListWidgetItem *item);

    // Handle the LED timeout.
    //void timeout_LED(void);

    // Handle the KEY timeout.
    // The method checks for a pressed key and invokes the appropriate function.
    //void timeout_KEY(void);

    //void on_volumeSlider_valueChanged(int value);

    void playbackOnGoing(bool value);

    bool playNext();
    bool playPrevious();

    void showMessageBoxAndClose(QString msg);

    void rfidTagDetected(QString tagId);

    //void on_PlayList_activated(int index);


    void loadCover(QString path);
    void loadCoverThroughFfmpeg(QString path);

    void on_btn_cd_pressed();
    void hardwareReady();
    void startPlayback(QString playlist, int track, int position);
    void on_btn_sleep_pressed();
    void on_btn_previousAlbum_pressed();
    void on_btn_nextAlbum_pressed();
    void on_btn_openAlbum_pressed();
    void on_btn_wakeup_pressed();
    void show_Playlist(int playListItem);

private:
    Ui::MainWindow *ui;

    // Holds the LED blink timeout value in ms
    const int Led_Timeout;
    // Holds the key poll timeout value in ms
    const int Key_Timeout;

    // The absoulte path/file name of the config file
    const QString myConfigFile;
    void swipeTriggered(QSwipeGesture*);
    bool gestureEvent(QGestureEvent *event);
};

#endif // MAINWINDOW_H
