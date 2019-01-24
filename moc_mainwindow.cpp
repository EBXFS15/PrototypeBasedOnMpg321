/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[49];
    char stringdata[716];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "startPlayback"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "playlist"
QT_MOC_LITERAL(4, 35, 5), // "track"
QT_MOC_LITERAL(5, 41, 8), // "position"
QT_MOC_LITERAL(6, 50, 19), // "on_btn_play_pressed"
QT_MOC_LITERAL(7, 70, 19), // "on_btn_stop_pressed"
QT_MOC_LITERAL(8, 90, 17), // "on_btn_ff_pressed"
QT_MOC_LITERAL(9, 108, 17), // "on_btn_RW_pressed"
QT_MOC_LITERAL(10, 126, 20), // "on_btn_pause_pressed"
QT_MOC_LITERAL(11, 147, 20), // "on_btn_sleep_pressed"
QT_MOC_LITERAL(12, 168, 18), // "on_playbackStarted"
QT_MOC_LITERAL(13, 187, 18), // "on_currentPosition"
QT_MOC_LITERAL(14, 206, 16), // "on_playbackEnded"
QT_MOC_LITERAL(15, 223, 28), // "on_btn_previousAlbum_pressed"
QT_MOC_LITERAL(16, 252, 24), // "on_btn_openAlbum_pressed"
QT_MOC_LITERAL(17, 277, 24), // "on_btn_nextAlbum_pressed"
QT_MOC_LITERAL(18, 302, 13), // "show_Playlist"
QT_MOC_LITERAL(19, 316, 12), // "playListItem"
QT_MOC_LITERAL(20, 329, 13), // "player_update"
QT_MOC_LITERAL(21, 343, 9), // "newStatus"
QT_MOC_LITERAL(22, 353, 12), // "initPlayList"
QT_MOC_LITERAL(23, 366, 21), // "on_PlayList_activated"
QT_MOC_LITERAL(24, 388, 4), // "arg1"
QT_MOC_LITERAL(25, 393, 22), // "get_PlayList_from_name"
QT_MOC_LITERAL(26, 416, 7), // "uint8_t"
QT_MOC_LITERAL(27, 424, 11), // "const char*"
QT_MOC_LITERAL(28, 436, 7), // "inNameP"
QT_MOC_LITERAL(29, 444, 22), // "get_PlayList_from_rfid"
QT_MOC_LITERAL(30, 467, 7), // "inRfidP"
QT_MOC_LITERAL(31, 475, 23), // "on_FileList_itemClicked"
QT_MOC_LITERAL(32, 499, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(33, 516, 4), // "item"
QT_MOC_LITERAL(34, 521, 11), // "timeout_LED"
QT_MOC_LITERAL(35, 533, 11), // "timeout_KEY"
QT_MOC_LITERAL(36, 545, 15), // "playbackOnGoing"
QT_MOC_LITERAL(37, 561, 5), // "value"
QT_MOC_LITERAL(38, 567, 8), // "playNext"
QT_MOC_LITERAL(39, 576, 12), // "playPrevious"
QT_MOC_LITERAL(40, 589, 22), // "showMessageBoxAndClose"
QT_MOC_LITERAL(41, 612, 3), // "msg"
QT_MOC_LITERAL(42, 616, 15), // "rfidTagDetected"
QT_MOC_LITERAL(43, 632, 5), // "tagId"
QT_MOC_LITERAL(44, 638, 9), // "loadCover"
QT_MOC_LITERAL(45, 648, 4), // "path"
QT_MOC_LITERAL(46, 653, 22), // "loadCoverThroughFfmpeg"
QT_MOC_LITERAL(47, 676, 17), // "on_btn_cd_pressed"
QT_MOC_LITERAL(48, 694, 21) // "on_btn_wakeup_pressed"

    },
    "MainWindow\0startPlayback\0\0playlist\0"
    "track\0position\0on_btn_play_pressed\0"
    "on_btn_stop_pressed\0on_btn_ff_pressed\0"
    "on_btn_RW_pressed\0on_btn_pause_pressed\0"
    "on_btn_sleep_pressed\0on_playbackStarted\0"
    "on_currentPosition\0on_playbackEnded\0"
    "on_btn_previousAlbum_pressed\0"
    "on_btn_openAlbum_pressed\0"
    "on_btn_nextAlbum_pressed\0show_Playlist\0"
    "playListItem\0player_update\0newStatus\0"
    "initPlayList\0on_PlayList_activated\0"
    "arg1\0get_PlayList_from_name\0uint8_t\0"
    "const char*\0inNameP\0get_PlayList_from_rfid\0"
    "inRfidP\0on_FileList_itemClicked\0"
    "QListWidgetItem*\0item\0timeout_LED\0"
    "timeout_KEY\0playbackOnGoing\0value\0"
    "playNext\0playPrevious\0showMessageBoxAndClose\0"
    "msg\0rfidTagDetected\0tagId\0loadCover\0"
    "path\0loadCoverThroughFfmpeg\0"
    "on_btn_cd_pressed\0on_btn_wakeup_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,  169,    2, 0x0a /* Public */,
       6,    0,  176,    2, 0x08 /* Private */,
       7,    0,  177,    2, 0x08 /* Private */,
       8,    0,  178,    2, 0x08 /* Private */,
       9,    0,  179,    2, 0x08 /* Private */,
      10,    0,  180,    2, 0x08 /* Private */,
      11,    0,  181,    2, 0x08 /* Private */,
      12,    0,  182,    2, 0x08 /* Private */,
      13,    1,  183,    2, 0x08 /* Private */,
      14,    0,  186,    2, 0x08 /* Private */,
      15,    0,  187,    2, 0x08 /* Private */,
      16,    0,  188,    2, 0x08 /* Private */,
      17,    0,  189,    2, 0x08 /* Private */,
      18,    1,  190,    2, 0x08 /* Private */,
      20,    1,  193,    2, 0x08 /* Private */,
      22,    0,  196,    2, 0x08 /* Private */,
      23,    1,  197,    2, 0x08 /* Private */,
      25,    1,  200,    2, 0x08 /* Private */,
      29,    1,  203,    2, 0x08 /* Private */,
      31,    1,  206,    2, 0x08 /* Private */,
      34,    0,  209,    2, 0x08 /* Private */,
      35,    0,  210,    2, 0x08 /* Private */,
      36,    1,  211,    2, 0x08 /* Private */,
      38,    0,  214,    2, 0x08 /* Private */,
      39,    0,  215,    2, 0x08 /* Private */,
      40,    1,  216,    2, 0x08 /* Private */,
      42,    1,  219,    2, 0x08 /* Private */,
      44,    1,  222,    2, 0x08 /* Private */,
      46,    1,  225,    2, 0x08 /* Private */,
      47,    0,  228,    2, 0x08 /* Private */,
      48,    0,  229,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    0x80000000 | 26, 0x80000000 | 27,   28,
    0x80000000 | 26, 0x80000000 | 27,   30,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   37,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void, QMetaType::QString,   43,
    QMetaType::Void, QMetaType::QString,   45,
    QMetaType::Void, QMetaType::QString,   45,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->startPlayback((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->on_btn_play_pressed(); break;
        case 2: _t->on_btn_stop_pressed(); break;
        case 3: _t->on_btn_ff_pressed(); break;
        case 4: _t->on_btn_RW_pressed(); break;
        case 5: _t->on_btn_pause_pressed(); break;
        case 6: _t->on_btn_sleep_pressed(); break;
        case 7: _t->on_playbackStarted(); break;
        case 8: _t->on_currentPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_playbackEnded(); break;
        case 10: _t->on_btn_previousAlbum_pressed(); break;
        case 11: _t->on_btn_openAlbum_pressed(); break;
        case 12: _t->on_btn_nextAlbum_pressed(); break;
        case 13: _t->show_Playlist((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->player_update((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->initPlayList(); break;
        case 16: _t->on_PlayList_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: { uint8_t _r = _t->get_PlayList_from_name((*reinterpret_cast< const char*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint8_t*>(_a[0]) = _r; }  break;
        case 18: { uint8_t _r = _t->get_PlayList_from_rfid((*reinterpret_cast< const char*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint8_t*>(_a[0]) = _r; }  break;
        case 19: _t->on_FileList_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 20: _t->timeout_LED(); break;
        case 21: _t->timeout_KEY(); break;
        case 22: _t->playbackOnGoing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: { bool _r = _t->playNext();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = _t->playPrevious();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: _t->showMessageBoxAndClose((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->rfidTagDetected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->loadCover((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->loadCoverThroughFfmpeg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->on_btn_cd_pressed(); break;
        case 30: _t->on_btn_wakeup_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 31;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
