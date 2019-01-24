/****************************************************************************
** Meta object code from reading C++ file 'continuetoplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "continuetoplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'continuetoplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ContinueToPlay_t {
    QByteArrayData data[11];
    char stringdata[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ContinueToPlay_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ContinueToPlay_t qt_meta_stringdata_ContinueToPlay = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ContinueToPlay"
QT_MOC_LITERAL(1, 15, 13), // "startPlayback"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "playlist"
QT_MOC_LITERAL(4, 39, 5), // "track"
QT_MOC_LITERAL(5, 45, 8), // "position"
QT_MOC_LITERAL(6, 54, 18), // "setCurrentPlaylist"
QT_MOC_LITERAL(7, 73, 15), // "setCurrentTrack"
QT_MOC_LITERAL(8, 89, 19), // "setPlaybackPosition"
QT_MOC_LITERAL(9, 109, 5), // "store"
QT_MOC_LITERAL(10, 115, 4) // "load"

    },
    "ContinueToPlay\0startPlayback\0\0playlist\0"
    "track\0position\0setCurrentPlaylist\0"
    "setCurrentTrack\0setPlaybackPosition\0"
    "store\0load"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ContinueToPlay[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   51,    2, 0x0a /* Public */,
       7,    1,   54,    2, 0x0a /* Public */,
       8,    1,   57,    2, 0x0a /* Public */,
       9,    0,   60,    2, 0x0a /* Public */,
      10,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ContinueToPlay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ContinueToPlay *_t = static_cast<ContinueToPlay *>(_o);
        switch (_id) {
        case 0: _t->startPlayback((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setCurrentPlaylist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setCurrentTrack((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setPlaybackPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->store(); break;
        case 5: _t->load(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ContinueToPlay::*_t)(QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ContinueToPlay::startPlayback)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ContinueToPlay::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ContinueToPlay.data,
      qt_meta_data_ContinueToPlay,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ContinueToPlay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContinueToPlay::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ContinueToPlay.stringdata))
        return static_cast<void*>(const_cast< ContinueToPlay*>(this));
    return QObject::qt_metacast(_clname);
}

int ContinueToPlay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ContinueToPlay::startPlayback(QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
