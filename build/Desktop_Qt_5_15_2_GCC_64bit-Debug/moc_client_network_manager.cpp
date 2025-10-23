/****************************************************************************
** Meta object code from reading C++ file 'client_network_manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../client/network/client_network_manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_network_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClientNetworkManager_t {
    QByteArrayData data[46];
    char stringdata0[816];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientNetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientNetworkManager_t qt_meta_stringdata_ClientNetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ClientNetworkManager"
QT_MOC_LITERAL(1, 21, 9), // "connected"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "disconnected"
QT_MOC_LITERAL(4, 45, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 59, 12), // "errorMessage"
QT_MOC_LITERAL(6, 72, 21), // "loginResponseReceived"
QT_MOC_LITERAL(7, 94, 7), // "success"
QT_MOC_LITERAL(8, 102, 7), // "message"
QT_MOC_LITERAL(9, 110, 24), // "registerResponseReceived"
QT_MOC_LITERAL(10, 135, 23), // "setUserResponseReceived"
QT_MOC_LITERAL(11, 159, 4), // "User"
QT_MOC_LITERAL(12, 164, 4), // "user"
QT_MOC_LITERAL(13, 169, 23), // "getUserResponseReceived"
QT_MOC_LITERAL(14, 193, 25), // "getAgendaResponseReceived"
QT_MOC_LITERAL(15, 219, 15), // "QVector<Agenda>"
QT_MOC_LITERAL(16, 235, 7), // "agendas"
QT_MOC_LITERAL(17, 243, 26), // "getContactResponseReceived"
QT_MOC_LITERAL(18, 270, 13), // "QVector<User>"
QT_MOC_LITERAL(19, 284, 25), // "getDeviceResponseReceived"
QT_MOC_LITERAL(20, 310, 8), // "successs"
QT_MOC_LITERAL(21, 319, 15), // "QVector<Device>"
QT_MOC_LITERAL(22, 335, 7), // "devices"
QT_MOC_LITERAL(23, 343, 25), // "addAgendaResponseReceived"
QT_MOC_LITERAL(24, 369, 25), // "addDeviceResponseReceived"
QT_MOC_LITERAL(25, 395, 28), // "removeDeviceResponseReceived"
QT_MOC_LITERAL(26, 424, 31), // "sendChatMessageResponseReceived"
QT_MOC_LITERAL(27, 456, 30), // "getChatHistoryResponseReceived"
QT_MOC_LITERAL(28, 487, 20), // "QVector<ChatMessage>"
QT_MOC_LITERAL(29, 508, 7), // "history"
QT_MOC_LITERAL(30, 516, 31), // "addSimulateDataResponseReceived"
QT_MOC_LITERAL(31, 548, 31), // "nonContactUsersResponseReceived"
QT_MOC_LITERAL(32, 580, 11), // "QList<User>"
QT_MOC_LITERAL(33, 592, 5), // "users"
QT_MOC_LITERAL(34, 598, 26), // "addContactResponseReceived"
QT_MOC_LITERAL(35, 625, 29), // "getAllDevicesResponseReceived"
QT_MOC_LITERAL(36, 655, 28), // "updateDeviceResponseReceived"
QT_MOC_LITERAL(37, 684, 22), // "newChatMessageReceived"
QT_MOC_LITERAL(38, 707, 9), // "contactId"
QT_MOC_LITERAL(39, 717, 8), // "senderId"
QT_MOC_LITERAL(40, 726, 11), // "onConnected"
QT_MOC_LITERAL(41, 738, 14), // "onDisconnected"
QT_MOC_LITERAL(42, 753, 15), // "onErrorOccurred"
QT_MOC_LITERAL(43, 769, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(44, 798, 5), // "error"
QT_MOC_LITERAL(45, 804, 11) // "onReadyRead"

    },
    "ClientNetworkManager\0connected\0\0"
    "disconnected\0errorOccurred\0errorMessage\0"
    "loginResponseReceived\0success\0message\0"
    "registerResponseReceived\0"
    "setUserResponseReceived\0User\0user\0"
    "getUserResponseReceived\0"
    "getAgendaResponseReceived\0QVector<Agenda>\0"
    "agendas\0getContactResponseReceived\0"
    "QVector<User>\0getDeviceResponseReceived\0"
    "successs\0QVector<Device>\0devices\0"
    "addAgendaResponseReceived\0"
    "addDeviceResponseReceived\0"
    "removeDeviceResponseReceived\0"
    "sendChatMessageResponseReceived\0"
    "getChatHistoryResponseReceived\0"
    "QVector<ChatMessage>\0history\0"
    "addSimulateDataResponseReceived\0"
    "nonContactUsersResponseReceived\0"
    "QList<User>\0users\0addContactResponseReceived\0"
    "getAllDevicesResponseReceived\0"
    "updateDeviceResponseReceived\0"
    "newChatMessageReceived\0contactId\0"
    "senderId\0onConnected\0onDisconnected\0"
    "onErrorOccurred\0QAbstractSocket::SocketError\0"
    "error\0onReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientNetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      21,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  139,    2, 0x06 /* Public */,
       3,    0,  140,    2, 0x06 /* Public */,
       4,    1,  141,    2, 0x06 /* Public */,
       6,    2,  144,    2, 0x06 /* Public */,
       9,    2,  149,    2, 0x06 /* Public */,
      10,    3,  154,    2, 0x06 /* Public */,
      13,    3,  161,    2, 0x06 /* Public */,
      14,    3,  168,    2, 0x06 /* Public */,
      17,    3,  175,    2, 0x06 /* Public */,
      19,    3,  182,    2, 0x06 /* Public */,
      23,    2,  189,    2, 0x06 /* Public */,
      24,    2,  194,    2, 0x06 /* Public */,
      25,    2,  199,    2, 0x06 /* Public */,
      26,    2,  204,    2, 0x06 /* Public */,
      27,    3,  209,    2, 0x06 /* Public */,
      30,    2,  216,    2, 0x06 /* Public */,
      31,    1,  221,    2, 0x06 /* Public */,
      34,    2,  224,    2, 0x06 /* Public */,
      35,    3,  229,    2, 0x06 /* Public */,
      36,    2,  236,    2, 0x06 /* Public */,
      37,    2,  241,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      40,    0,  246,    2, 0x08 /* Private */,
      41,    0,  247,    2, 0x08 /* Private */,
      42,    1,  248,    2, 0x08 /* Private */,
      45,    0,  251,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    7,    8,   12,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    7,    8,   12,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 15, QMetaType::QString,    7,   16,    8,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 18, QMetaType::QString,    7,   16,    8,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 21, QMetaType::QString,   20,   22,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 28, QMetaType::QString,    7,   29,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 21,    7,    8,   22,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   38,   39,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 43,   44,
    QMetaType::Void,

       0        // eod
};

void ClientNetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientNetworkManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->loginResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->registerResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->setUserResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< User(*)>(_a[3]))); break;
        case 6: _t->getUserResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< User(*)>(_a[3]))); break;
        case 7: _t->getAgendaResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Agenda>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 8: _t->getContactResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<User>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: _t->getDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Device>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->addAgendaResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->addDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->removeDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->sendChatMessageResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: _t->getChatHistoryResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<ChatMessage>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 15: _t->addSimulateDataResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 16: _t->nonContactUsersResponseReceived((*reinterpret_cast< const QList<User>(*)>(_a[1]))); break;
        case 17: _t->addContactResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: _t->getAllDevicesResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVector<Device>(*)>(_a[3]))); break;
        case 19: _t->updateDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 20: _t->newChatMessageReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->onConnected(); break;
        case 22: _t->onDisconnected(); break;
        case 23: _t->onErrorOccurred((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 24: _t->onReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< User >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< User >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Agenda> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<User> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<User> >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 23:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientNetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::loginResponseReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::registerResponseReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & , User );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::setUserResponseReceived)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & , User );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getUserResponseReceived)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QVector<Agenda> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getAgendaResponseReceived)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QVector<User> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getContactResponseReceived)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QVector<Device> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getDeviceResponseReceived)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::addAgendaResponseReceived)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::addDeviceResponseReceived)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::removeDeviceResponseReceived)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::sendChatMessageResponseReceived)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QVector<ChatMessage> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getChatHistoryResponseReceived)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::addSimulateDataResponseReceived)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(const QList<User> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::nonContactUsersResponseReceived)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::addContactResponseReceived)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & , const QVector<Device> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::getAllDevicesResponseReceived)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::updateDeviceResponseReceived)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (ClientNetworkManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientNetworkManager::newChatMessageReceived)) {
                *result = 20;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ClientNetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ClientNetworkManager.data,
    qt_meta_data_ClientNetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClientNetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientNetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientNetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ClientNetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void ClientNetworkManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ClientNetworkManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ClientNetworkManager::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ClientNetworkManager::loginResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ClientNetworkManager::registerResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ClientNetworkManager::setUserResponseReceived(bool _t1, const QString & _t2, User _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ClientNetworkManager::getUserResponseReceived(bool _t1, const QString & _t2, User _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ClientNetworkManager::getAgendaResponseReceived(bool _t1, const QVector<Agenda> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ClientNetworkManager::getContactResponseReceived(bool _t1, const QVector<User> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ClientNetworkManager::getDeviceResponseReceived(bool _t1, const QVector<Device> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ClientNetworkManager::addAgendaResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ClientNetworkManager::addDeviceResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ClientNetworkManager::removeDeviceResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ClientNetworkManager::sendChatMessageResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void ClientNetworkManager::getChatHistoryResponseReceived(bool _t1, const QVector<ChatMessage> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void ClientNetworkManager::addSimulateDataResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void ClientNetworkManager::nonContactUsersResponseReceived(const QList<User> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void ClientNetworkManager::addContactResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void ClientNetworkManager::getAllDevicesResponseReceived(bool _t1, const QString & _t2, const QVector<Device> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void ClientNetworkManager::updateDeviceResponseReceived(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void ClientNetworkManager::newChatMessageReceived(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
