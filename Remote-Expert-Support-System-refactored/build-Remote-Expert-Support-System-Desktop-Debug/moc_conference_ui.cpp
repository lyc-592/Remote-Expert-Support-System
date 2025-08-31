/****************************************************************************
** Meta object code from reading C++ file 'conference_ui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Remote-Expert-Support-System/ui/conference/conference_ui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conference_ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConferenceUi_t {
    QByteArrayData data[13];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceUi_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceUi_t qt_meta_stringdata_ConferenceUi = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ConferenceUi"
QT_MOC_LITERAL(1, 13, 28), // "on_startServerButton_clicked"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 24), // "on_connectButton_clicked"
QT_MOC_LITERAL(4, 68, 21), // "on_sendButton_clicked"
QT_MOC_LITERAL(5, 90, 23), // "on_attachButton_clicked"
QT_MOC_LITERAL(6, 114, 17), // "onClientConnected"
QT_MOC_LITERAL(7, 132, 20), // "onClientDisconnected"
QT_MOC_LITERAL(8, 153, 17), // "onMessageReceived"
QT_MOC_LITERAL(9, 171, 7), // "message"
QT_MOC_LITERAL(10, 179, 23), // "onServerClientConnected"
QT_MOC_LITERAL(11, 203, 26), // "onServerClientDisconnected"
QT_MOC_LITERAL(12, 230, 18) // "onServerNewMessage"

    },
    "ConferenceUi\0on_startServerButton_clicked\0"
    "\0on_connectButton_clicked\0"
    "on_sendButton_clicked\0on_attachButton_clicked\0"
    "onClientConnected\0onClientDisconnected\0"
    "onMessageReceived\0message\0"
    "onServerClientConnected\0"
    "onServerClientDisconnected\0"
    "onServerNewMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceUi[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
      10,    0,   73,    2, 0x08 /* Private */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    1,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    9,

       0        // eod
};

void ConferenceUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConferenceUi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_startServerButton_clicked(); break;
        case 1: _t->on_connectButton_clicked(); break;
        case 2: _t->on_sendButton_clicked(); break;
        case 3: _t->on_attachButton_clicked(); break;
        case 4: _t->onClientConnected(); break;
        case 5: _t->onClientDisconnected(); break;
        case 6: _t->onMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->onServerClientConnected(); break;
        case 8: _t->onServerClientDisconnected(); break;
        case 9: _t->onServerNewMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConferenceUi::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ConferenceUi.data,
    qt_meta_data_ConferenceUi,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConferenceUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceUi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceUi.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ConferenceUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
