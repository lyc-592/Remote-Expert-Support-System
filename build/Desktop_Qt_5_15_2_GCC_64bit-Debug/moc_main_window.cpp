/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ui/index/factory/main_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[32];
    char stringdata0[469];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "onMenuChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "index"
QT_MOC_LITERAL(4, 32, 14), // "onIndexChanged"
QT_MOC_LITERAL(5, 47, 16), // "onAgendaReceived"
QT_MOC_LITERAL(6, 64, 7), // "success"
QT_MOC_LITERAL(7, 72, 15), // "QVector<Agenda>"
QT_MOC_LITERAL(8, 88, 7), // "agendas"
QT_MOC_LITERAL(9, 96, 7), // "message"
QT_MOC_LITERAL(10, 104, 16), // "onContactClicked"
QT_MOC_LITERAL(11, 121, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(12, 138, 4), // "item"
QT_MOC_LITERAL(13, 143, 17), // "onContactReceived"
QT_MOC_LITERAL(14, 161, 13), // "QVector<User>"
QT_MOC_LITERAL(15, 175, 8), // "contacts"
QT_MOC_LITERAL(16, 184, 16), // "onDeviceReceived"
QT_MOC_LITERAL(17, 201, 15), // "QVector<Device>"
QT_MOC_LITERAL(18, 217, 7), // "devices"
QT_MOC_LITERAL(19, 225, 19), // "onAddDeviceReceived"
QT_MOC_LITERAL(20, 245, 19), // "onAddAgendaReceived"
QT_MOC_LITERAL(21, 265, 22), // "onRemoveDeviceReceived"
QT_MOC_LITERAL(22, 288, 25), // "onAddSimulateDataReceived"
QT_MOC_LITERAL(23, 314, 25), // "onAddContactButtonClicked"
QT_MOC_LITERAL(24, 340, 25), // "onNonContactUsersReceived"
QT_MOC_LITERAL(25, 366, 11), // "QList<User>"
QT_MOC_LITERAL(26, 378, 5), // "users"
QT_MOC_LITERAL(27, 384, 28), // "onAddContactResponseReceived"
QT_MOC_LITERAL(28, 413, 22), // "onUpdateDeviceReceived"
QT_MOC_LITERAL(29, 436, 16), // "onNewChatMessage"
QT_MOC_LITERAL(30, 453, 6), // "chatId"
QT_MOC_LITERAL(31, 460, 8) // "senderId"

    },
    "MainWindow\0onMenuChanged\0\0index\0"
    "onIndexChanged\0onAgendaReceived\0success\0"
    "QVector<Agenda>\0agendas\0message\0"
    "onContactClicked\0QListWidgetItem*\0"
    "item\0onContactReceived\0QVector<User>\0"
    "contacts\0onDeviceReceived\0QVector<Device>\0"
    "devices\0onAddDeviceReceived\0"
    "onAddAgendaReceived\0onRemoveDeviceReceived\0"
    "onAddSimulateDataReceived\0"
    "onAddContactButtonClicked\0"
    "onNonContactUsersReceived\0QList<User>\0"
    "users\0onAddContactResponseReceived\0"
    "onUpdateDeviceReceived\0onNewChatMessage\0"
    "chatId\0senderId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x08 /* Private */,
       4,    1,   92,    2, 0x08 /* Private */,
       5,    3,   95,    2, 0x08 /* Private */,
      10,    1,  102,    2, 0x08 /* Private */,
      13,    3,  105,    2, 0x08 /* Private */,
      16,    3,  112,    2, 0x08 /* Private */,
      19,    2,  119,    2, 0x08 /* Private */,
      20,    2,  124,    2, 0x08 /* Private */,
      21,    2,  129,    2, 0x08 /* Private */,
      22,    2,  134,    2, 0x08 /* Private */,
      23,    0,  139,    2, 0x08 /* Private */,
      24,    1,  140,    2, 0x08 /* Private */,
      27,    2,  143,    2, 0x08 /* Private */,
      28,    2,  148,    2, 0x08 /* Private */,
      29,    2,  153,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 7, QMetaType::QString,    6,    8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 14, QMetaType::QString,    6,   15,    9,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 17, QMetaType::QString,    6,   18,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   30,   31,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onMenuChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onAgendaReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Agenda>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->onContactClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->onContactReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<User>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: _t->onDeviceReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Device>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: _t->onAddDeviceReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->onAddAgendaReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->onRemoveDeviceReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->onAddSimulateDataReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->onAddContactButtonClicked(); break;
        case 11: _t->onNonContactUsersReceived((*reinterpret_cast< const QList<User>(*)>(_a[1]))); break;
        case 12: _t->onAddContactResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->onUpdateDeviceReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: _t->onNewChatMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Agenda> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<User> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<User> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
