#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextStream>
#include <QDebug>

QProcess mpg321;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    //mpg321.connect(mpg321,SIGNAL(readyRead()),this,SLOT(player_update()));
    connect(&mpg321, SIGNAL(readyRead()), this, SLOT(player_update()));
    mpg321.start("mpg321", QStringList() << "-R" << "abc");
    mpg321.write("L /media/test.mp3");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    mpg321.write("Q");
    mpg321.closeWriteChannel();
    delete ui;
}

void MainWindow::player_update()
{

    ui->textEdit->append(mpg321.readAll());
}


void MainWindow::on_btn_play_pressed()
{
    mpg321.write("L /media/test.mp3");
}

void MainWindow::on_btn_stop_pressed()
{
    mpg321.write("S");
}

void MainWindow::on_btn_ff_pressed()
{
    mpg321.write("J +100");
}

void MainWindow::on_btn_RW_pressed()
{
    mpg321.write("J -100");
}

void MainWindow::on_btn_close_pressed()
{
}

void MainWindow::on_btn_pause_pressed()
{
    mpg321.write("P");
}
