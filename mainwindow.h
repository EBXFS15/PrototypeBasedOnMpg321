#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <stdint.h>
#include "mplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    mplayer * player;
    ~MainWindow();

private slots:
    void on_btn_play_pressed();

    void on_btn_stop_pressed();

    void on_btn_ff_pressed();

    void on_btn_RW_pressed();

    void on_btn_close_pressed();

    void on_btn_pause_pressed();

    void on_playbackStarted();

    void on_currentPosition(int position);

    void on_playbackEnded();

    void player_update(QString newStatus);

    // Retrieve all play lists from the configuration
    // file an add the names to play list GUI element.
    void initPlayList(void);

    // Is called for a selection in the play list.
    // The method shows the file list for the play list.
    void on_PlayList_activated(const QString &arg1);

    // Get the play list number from the given name.
    uint8_t get_PlayList_from_name(const char* inNameP);

    // Get the play list number from the given RFID number.
    uint8_t get_PlayList_from_rfid(const char* inRfidP);

    // Play the selected file from the file list.
    void on_FileList_itemClicked(QListWidgetItem *item);

    // Handle the LED timeout.
    void timeout_LED(void);

    // Handle the KEY timeout.
    // The method checks for a pressed key and invokes the appropriate function.
    void timeout_KEY(void);

    void on_volumeSlider_valueChanged(int value);

    void playbackOnGoing(bool value);

    bool playNext();

    void showMessageBoxAndClose(QString msg);

private:
    Ui::MainWindow *ui;

    // Holds the GPIO for key SW1
    const int Key_SW1_GPIO;
    // Holds the GPIO for key SW2
    const int Key_SW2_GPIO;
    // Holds the GPIO for key SW3
    const int Key_SW3_GPIO;
    // Holds the GPIO for key SW4
    const int Key_SW4_GPIO;
    // Holds the GPIO for key SW5
    const int Key_SW5_GPIO;
    // Holds the GPIO for LED1
    const int Led_1_GPIO;
    // Holds the GPIO for LED2
    const int Led_2_GPIO;
    // Holds the LED blink timeout value in ms
    const int Led_Timeout;
    // Holds the key poll timeout value in ms
    const int Key_Timeout;

    // The absoulte path/file name of the config file
    QString myConfigFile;
};

#endif // MAINWINDOW_H
