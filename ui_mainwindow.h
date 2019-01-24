/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_wakeup;
    QPushButton *btn_previousAlbum;
    QPushButton *btn_openAlbum;
    QPushButton *btn_nextAlbum;
    QPushButton *btn_singleRepeat;
    QPushButton *btn_cd;
    QVBoxLayout *verticalLayout_5;
    QComboBox *PlayList;
    QHBoxLayout *horizontalLayout_3;
    QLabel *mediaImage;
    QListWidget *FileList;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btn_sleep;
    QPushButton *btn_play;
    QPushButton *btn_pause;
    QPushButton *btn_RW;
    QPushButton *btn_ff;
    QPushButton *btn_stop;
    QScrollBar *hBar_position;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(665, 370);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMaximumSize(QSize(2000, 650));
        MainWindow->setMouseTracking(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 9402, 1189));
        horizontalLayout_4 = new QHBoxLayout(verticalLayoutWidget);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 9, -1, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btn_wakeup = new QPushButton(verticalLayoutWidget);
        btn_wakeup->setObjectName(QStringLiteral("btn_wakeup"));
        btn_wakeup->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_wakeup->sizePolicy().hasHeightForWidth());
        btn_wakeup->setSizePolicy(sizePolicy1);
        btn_wakeup->setMinimumSize(QSize(32, 32));
        btn_wakeup->setMaximumSize(QSize(84, 84));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Resources/icons/media-GoodMorning.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_wakeup->setIcon(icon);
        btn_wakeup->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(btn_wakeup);

        btn_previousAlbum = new QPushButton(verticalLayoutWidget);
        btn_previousAlbum->setObjectName(QStringLiteral("btn_previousAlbum"));
        btn_previousAlbum->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_previousAlbum->sizePolicy().hasHeightForWidth());
        btn_previousAlbum->setSizePolicy(sizePolicy1);
        btn_previousAlbum->setMinimumSize(QSize(32, 32));
        btn_previousAlbum->setMaximumSize(QSize(84, 84));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/Resources/icons/media-previousPlaylist.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_previousAlbum->setIcon(icon1);
        btn_previousAlbum->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(btn_previousAlbum);

        btn_openAlbum = new QPushButton(verticalLayoutWidget);
        btn_openAlbum->setObjectName(QStringLiteral("btn_openAlbum"));
        btn_openAlbum->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_openAlbum->sizePolicy().hasHeightForWidth());
        btn_openAlbum->setSizePolicy(sizePolicy1);
        btn_openAlbum->setMinimumSize(QSize(32, 32));
        btn_openAlbum->setMaximumSize(QSize(84, 84));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/Resources/icons/media-open.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_openAlbum->setIcon(icon2);
        btn_openAlbum->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(btn_openAlbum);

        btn_nextAlbum = new QPushButton(verticalLayoutWidget);
        btn_nextAlbum->setObjectName(QStringLiteral("btn_nextAlbum"));
        btn_nextAlbum->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_nextAlbum->sizePolicy().hasHeightForWidth());
        btn_nextAlbum->setSizePolicy(sizePolicy1);
        btn_nextAlbum->setMinimumSize(QSize(32, 32));
        btn_nextAlbum->setMaximumSize(QSize(84, 84));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Resources/icons/media-nextPlaylist.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_nextAlbum->setIcon(icon3);
        btn_nextAlbum->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(btn_nextAlbum);

        btn_singleRepeat = new QPushButton(verticalLayoutWidget);
        btn_singleRepeat->setObjectName(QStringLiteral("btn_singleRepeat"));
        sizePolicy.setHeightForWidth(btn_singleRepeat->sizePolicy().hasHeightForWidth());
        btn_singleRepeat->setSizePolicy(sizePolicy);
        btn_singleRepeat->setMinimumSize(QSize(32, 32));
        btn_singleRepeat->setMaximumSize(QSize(84, 84));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/Resources/icons/SingleDoNotRepeat.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QStringLiteral(":/icons/Resources/icons/SingleRepeat.svg"), QSize(), QIcon::Normal, QIcon::On);
        icon4.addFile(QStringLiteral(":/icons/Resources/icons/SingleRepeat.svg"), QSize(), QIcon::Active, QIcon::On);
        icon4.addFile(QStringLiteral(":/icons/Resources/icons/SingleRepeat.svg"), QSize(), QIcon::Selected, QIcon::On);
        btn_singleRepeat->setIcon(icon4);
        btn_singleRepeat->setIconSize(QSize(32, 32));
        btn_singleRepeat->setCheckable(true);

        verticalLayout->addWidget(btn_singleRepeat);

        btn_cd = new QPushButton(verticalLayoutWidget);
        btn_cd->setObjectName(QStringLiteral("btn_cd"));
        sizePolicy.setHeightForWidth(btn_cd->sizePolicy().hasHeightForWidth());
        btn_cd->setSizePolicy(sizePolicy);
        btn_cd->setMinimumSize(QSize(32, 32));
        btn_cd->setMaximumSize(QSize(84, 84));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/Resources/icons/doNotRepeat.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon5.addFile(QStringLiteral(":/icons/Resources/icons/repeat.svg"), QSize(), QIcon::Normal, QIcon::On);
        icon5.addFile(QStringLiteral(":/icons/Resources/icons/repeat.svg"), QSize(), QIcon::Active, QIcon::On);
        icon5.addFile(QStringLiteral(":/icons/Resources/icons/repeat.svg"), QSize(), QIcon::Selected, QIcon::On);
        btn_cd->setIcon(icon5);
        btn_cd->setIconSize(QSize(32, 32));
        btn_cd->setCheckable(true);

        verticalLayout->addWidget(btn_cd);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 0, 0, -1);
        PlayList = new QComboBox(verticalLayoutWidget);
        PlayList->setObjectName(QStringLiteral("PlayList"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(PlayList->sizePolicy().hasHeightForWidth());
        PlayList->setSizePolicy(sizePolicy2);
        PlayList->setMinimumSize(QSize(300, 25));
        QFont font;
        font.setPointSize(20);
        PlayList->setFont(font);
        PlayList->setStyleSheet(QLatin1String("QScrollBar:vertical {\n"
"border-radius: 8px;\n"
"background: gray;\n"
"width: 48px;\n"
"margin: 0px 0px 0px 0px;\n"
"}"));
        PlayList->setIconSize(QSize(100, 100));
        PlayList->setFrame(true);

        verticalLayout_5->addWidget(PlayList);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, 0, -1);
        mediaImage = new QLabel(verticalLayoutWidget);
        mediaImage->setObjectName(QStringLiteral("mediaImage"));
        mediaImage->setEnabled(true);
        sizePolicy1.setHeightForWidth(mediaImage->sizePolicy().hasHeightForWidth());
        mediaImage->setSizePolicy(sizePolicy1);
        mediaImage->setMinimumSize(QSize(250, 250));
        mediaImage->setMaximumSize(QSize(250, 250));
        mediaImage->setCursor(QCursor(Qt::ArrowCursor));
        mediaImage->setMouseTracking(false);
        mediaImage->setAcceptDrops(false);
        mediaImage->setFrameShape(QFrame::StyledPanel);
        mediaImage->setPixmap(QPixmap(QString::fromUtf8(":/icons/Resources/icons/logo.png")));
        mediaImage->setScaledContents(true);

        horizontalLayout_3->addWidget(mediaImage);

        FileList = new QListWidget(verticalLayoutWidget);
        FileList->setObjectName(QStringLiteral("FileList"));
        sizePolicy1.setHeightForWidth(FileList->sizePolicy().hasHeightForWidth());
        FileList->setSizePolicy(sizePolicy1);
        FileList->setMinimumSize(QSize(200, 0));
        FileList->setMaximumSize(QSize(350, 450));
        FileList->setBaseSize(QSize(460, 460));
        QFont font1;
        font1.setPointSize(16);
        FileList->setFont(font1);
        FileList->setTextElideMode(Qt::ElideLeft);
        FileList->setBatchSize(100);

        horizontalLayout_3->addWidget(FileList);


        verticalLayout_5->addLayout(horizontalLayout_3);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_3->setContentsMargins(0, 0, 0, -1);
        btn_sleep = new QPushButton(verticalLayoutWidget);
        btn_sleep->setObjectName(QStringLiteral("btn_sleep"));
        btn_sleep->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_sleep->sizePolicy().hasHeightForWidth());
        btn_sleep->setSizePolicy(sizePolicy1);
        btn_sleep->setMinimumSize(QSize(32, 32));
        btn_sleep->setMaximumSize(QSize(86, 86));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/Resources/icons/media-GoodNight.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_sleep->setIcon(icon6);
        btn_sleep->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_sleep);

        btn_play = new QPushButton(verticalLayoutWidget);
        btn_play->setObjectName(QStringLiteral("btn_play"));
        btn_play->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_play->sizePolicy().hasHeightForWidth());
        btn_play->setSizePolicy(sizePolicy1);
        btn_play->setMinimumSize(QSize(32, 32));
        btn_play->setMaximumSize(QSize(86, 86));
        btn_play->setContextMenuPolicy(Qt::NoContextMenu);
        btn_play->setAcceptDrops(false);
        btn_play->setAutoFillBackground(false);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/play"), QSize(), QIcon::Normal, QIcon::Off);
        btn_play->setIcon(icon7);
        btn_play->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_play);

        btn_pause = new QPushButton(verticalLayoutWidget);
        btn_pause->setObjectName(QStringLiteral("btn_pause"));
        btn_pause->setEnabled(false);
        sizePolicy1.setHeightForWidth(btn_pause->sizePolicy().hasHeightForWidth());
        btn_pause->setSizePolicy(sizePolicy1);
        btn_pause->setMinimumSize(QSize(32, 32));
        btn_pause->setMaximumSize(QSize(86, 86));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/pause"), QSize(), QIcon::Normal, QIcon::Off);
        btn_pause->setIcon(icon8);
        btn_pause->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_pause);

        btn_RW = new QPushButton(verticalLayoutWidget);
        btn_RW->setObjectName(QStringLiteral("btn_RW"));
        btn_RW->setEnabled(false);
        sizePolicy1.setHeightForWidth(btn_RW->sizePolicy().hasHeightForWidth());
        btn_RW->setSizePolicy(sizePolicy1);
        btn_RW->setMinimumSize(QSize(32, 32));
        btn_RW->setMaximumSize(QSize(86, 86));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/backward"), QSize(), QIcon::Normal, QIcon::Off);
        btn_RW->setIcon(icon9);
        btn_RW->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_RW);

        btn_ff = new QPushButton(verticalLayoutWidget);
        btn_ff->setObjectName(QStringLiteral("btn_ff"));
        btn_ff->setEnabled(false);
        sizePolicy1.setHeightForWidth(btn_ff->sizePolicy().hasHeightForWidth());
        btn_ff->setSizePolicy(sizePolicy1);
        btn_ff->setMinimumSize(QSize(32, 32));
        btn_ff->setMaximumSize(QSize(86, 86));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/forward"), QSize(), QIcon::Normal, QIcon::Off);
        btn_ff->setIcon(icon10);
        btn_ff->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_ff);

        btn_stop = new QPushButton(verticalLayoutWidget);
        btn_stop->setObjectName(QStringLiteral("btn_stop"));
        btn_stop->setEnabled(false);
        sizePolicy1.setHeightForWidth(btn_stop->sizePolicy().hasHeightForWidth());
        btn_stop->setSizePolicy(sizePolicy1);
        btn_stop->setMinimumSize(QSize(32, 32));
        btn_stop->setMaximumSize(QSize(86, 86));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/stop"), QSize(), QIcon::Normal, QIcon::Off);
        btn_stop->setIcon(icon11);
        btn_stop->setIconSize(QSize(32, 32));

        verticalLayout_3->addWidget(btn_stop);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        hBar_position = new QScrollBar(verticalLayoutWidget);
        hBar_position->setObjectName(QStringLiteral("hBar_position"));
        sizePolicy.setHeightForWidth(hBar_position->sizePolicy().hasHeightForWidth());
        hBar_position->setSizePolicy(sizePolicy);
        hBar_position->setMinimumSize(QSize(600, 13));
        hBar_position->setAutoFillBackground(false);
        hBar_position->setMaximum(1000);
        hBar_position->setValue(0);
        hBar_position->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(hBar_position);

        verticalSpacer = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout_4->addLayout(verticalLayout_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        btn_wakeup->setText(QString());
        btn_previousAlbum->setText(QString());
        btn_openAlbum->setText(QString());
        btn_nextAlbum->setText(QString());
        btn_singleRepeat->setText(QString());
        btn_cd->setText(QString());
        mediaImage->setText(QString());
        btn_sleep->setText(QString());
        btn_play->setText(QString());
        btn_pause->setText(QString());
        btn_RW->setText(QString());
        btn_ff->setText(QString());
        btn_stop->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
