/****************************************************************************
** Meta object code from reading C++ file 'mplayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mplayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mplayer_t {
    QByteArrayData data[29];
    char stringdata[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mplayer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mplayer_t qt_meta_stringdata_mplayer = {
    {
QT_MOC_LITERAL(0, 0, 7), // "mplayer"
QT_MOC_LITERAL(1, 8, 12), // "playbackInfo"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "filename"
QT_MOC_LITERAL(4, 31, 13), // "statusChanged"
QT_MOC_LITERAL(5, 45, 9), // "newStatus"
QT_MOC_LITERAL(6, 55, 13), // "playbackEnded"
QT_MOC_LITERAL(7, 69, 16), // "playbackPosition"
QT_MOC_LITERAL(8, 86, 15), // "playbackStarted"
QT_MOC_LITERAL(9, 102, 8), // "loadFile"
QT_MOC_LITERAL(10, 111, 4), // "path"
QT_MOC_LITERAL(11, 116, 4), // "play"
QT_MOC_LITERAL(12, 121, 4), // "stop"
QT_MOC_LITERAL(13, 126, 5), // "pause"
QT_MOC_LITERAL(14, 132, 3), // "pos"
QT_MOC_LITERAL(15, 136, 10), // "percentage"
QT_MOC_LITERAL(16, 147, 2), // "ff"
QT_MOC_LITERAL(17, 150, 6), // "frames"
QT_MOC_LITERAL(18, 157, 2), // "rw"
QT_MOC_LITERAL(19, 160, 9), // "setVolume"
QT_MOC_LITERAL(20, 170, 5), // "value"
QT_MOC_LITERAL(21, 176, 12), // "startProcess"
QT_MOC_LITERAL(22, 189, 5), // "winId"
QT_MOC_LITERAL(23, 195, 13), // "player_update"
QT_MOC_LITERAL(24, 209, 13), // "parsePosition"
QT_MOC_LITERAL(25, 223, 4), // "line"
QT_MOC_LITERAL(26, 228, 9), // "parseLine"
QT_MOC_LITERAL(27, 238, 19), // "sendCommandToPlayer"
QT_MOC_LITERAL(28, 258, 3) // "cmd"

    },
    "mplayer\0playbackInfo\0\0filename\0"
    "statusChanged\0newStatus\0playbackEnded\0"
    "playbackPosition\0playbackStarted\0"
    "loadFile\0path\0play\0stop\0pause\0pos\0"
    "percentage\0ff\0frames\0rw\0setVolume\0"
    "value\0startProcess\0winId\0player_update\0"
    "parsePosition\0line\0parseLine\0"
    "sendCommandToPlayer\0cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mplayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       4,    1,  112,    2, 0x06 /* Public */,
       6,    0,  115,    2, 0x06 /* Public */,
       7,    1,  116,    2, 0x06 /* Public */,
       8,    0,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  120,    2, 0x0a /* Public */,
      11,    0,  123,    2, 0x0a /* Public */,
      12,    0,  124,    2, 0x0a /* Public */,
      13,    0,  125,    2, 0x0a /* Public */,
      14,    1,  126,    2, 0x0a /* Public */,
      16,    1,  129,    2, 0x0a /* Public */,
      18,    1,  132,    2, 0x0a /* Public */,
      19,    1,  135,    2, 0x0a /* Public */,
      21,    1,  138,    2, 0x0a /* Public */,
      21,    0,  141,    2, 0x2a /* Public | MethodCloned */,
      23,    0,  142,    2, 0x08 /* Private */,
      24,    1,  143,    2, 0x08 /* Private */,
      26,    1,  146,    2, 0x08 /* Private */,
      27,    1,  149,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   28,

       0        // eod
};

void mplayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mplayer *_t = static_cast<mplayer *>(_o);
        switch (_id) {
        case 0: _t->playbackInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->playbackEnded(); break;
        case 3: _t->playbackPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->playbackStarted(); break;
        case 5: _t->loadFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->play(); break;
        case 7: _t->stop(); break;
        case 8: _t->pause(); break;
        case 9: _t->pos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->ff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->rw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->startProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->startProcess(); break;
        case 15: _t->player_update(); break;
        case 16: _t->parsePosition((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->parseLine((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->sendCommandToPlayer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (mplayer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mplayer::playbackInfo)) {
                *result = 0;
            }
        }
        {
            typedef void (mplayer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mplayer::statusChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (mplayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mplayer::playbackEnded)) {
                *result = 2;
            }
        }
        {
            typedef void (mplayer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mplayer::playbackPosition)) {
                *result = 3;
            }
        }
        {
            typedef void (mplayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mplayer::playbackStarted)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject mplayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mplayer.data,
      qt_meta_data_mplayer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mplayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mplayer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mplayer.stringdata))
        return static_cast<void*>(const_cast< mplayer*>(this));
    return QObject::qt_metacast(_clname);
}

int mplayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void mplayer::playbackInfo(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mplayer::statusChanged(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void mplayer::playbackEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void mplayer::playbackPosition(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void mplayer::playbackStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
