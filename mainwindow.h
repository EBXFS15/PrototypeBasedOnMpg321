#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_play_pressed();

    void on_btn_stop_pressed();

    void on_btn_ff_pressed();

    void on_btn_RW_pressed();

    void on_btn_close_pressed();

    void on_btn_pause_pressed();

    void player_update();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
