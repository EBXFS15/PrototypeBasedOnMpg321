#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <stdint.h>
#include "fileplayer.h"
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

private:
    Ui::MainWindow *ui;    
    char* myActualPlayedSongP;
};

#endif // MAINWINDOW_H
