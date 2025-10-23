/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../client/core/client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Client_t {
    QByteArrayData data[64];
    char stringdata0[1132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Client_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Client_t qt_meta_stringdata_Client = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Client"
QT_MOC_LITERAL(1, 7, 9), // "connected"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 12), // "disconnected"
QT_MOC_LITERAL(4, 31, 16), // "connectionFailed"
QT_MOC_LITERAL(5, 48, 15), // "messageReceived"
QT_MOC_LITERAL(6, 64, 7), // "message"
QT_MOC_LITERAL(7, 72, 11), // "loginResult"
QT_MOC_LITERAL(8, 84, 7), // "success"
QT_MOC_LITERAL(9, 92, 14), // "registerResult"
QT_MOC_LITERAL(10, 107, 13), // "setUserResult"
QT_MOC_LITERAL(11, 121, 4), // "User"
QT_MOC_LITERAL(12, 126, 4), // "user"
QT_MOC_LITERAL(13, 131, 13), // "getUserResult"
QT_MOC_LITERAL(14, 145, 8), // "userData"
QT_MOC_LITERAL(15, 154, 12), // "agendaResult"
QT_MOC_LITERAL(16, 167, 15), // "QVector<Agenda>"
QT_MOC_LITERAL(17, 183, 7), // "agendas"
QT_MOC_LITERAL(18, 191, 13), // "contactResult"
QT_MOC_LITERAL(19, 205, 13), // "QVector<User>"
QT_MOC_LITERAL(20, 219, 8), // "contacts"
QT_MOC_LITERAL(21, 228, 12), // "deviceResult"
QT_MOC_LITERAL(22, 241, 15), // "QVector<Device>"
QT_MOC_LITERAL(23, 257, 7), // "devices"
QT_MOC_LITERAL(24, 265, 15), // "addDeviceResult"
QT_MOC_LITERAL(25, 281, 18), // "removeDeviceResult"
QT_MOC_LITERAL(26, 300, 15), // "addAgendaResult"
QT_MOC_LITERAL(27, 316, 21), // "sendChatMessageResult"
QT_MOC_LITERAL(28, 338, 20), // "getChatHistoryResult"
QT_MOC_LITERAL(29, 359, 20), // "QVector<ChatMessage>"
QT_MOC_LITERAL(30, 380, 7), // "history"
QT_MOC_LITERAL(31, 388, 21), // "addSimulateDataResult"
QT_MOC_LITERAL(32, 410, 23), // "nonContactUsersReceived"
QT_MOC_LITERAL(33, 434, 11), // "QList<User>"
QT_MOC_LITERAL(34, 446, 5), // "users"
QT_MOC_LITERAL(35, 452, 16), // "addContactResult"
QT_MOC_LITERAL(36, 469, 19), // "getAllDevicesResult"
QT_MOC_LITERAL(37, 489, 18), // "updateDeviceResult"
QT_MOC_LITERAL(38, 508, 14), // "newChatMessage"
QT_MOC_LITERAL(39, 523, 9), // "contactId"
QT_MOC_LITERAL(40, 533, 8), // "senderId"
QT_MOC_LITERAL(41, 542, 11), // "onConnected"
QT_MOC_LITERAL(42, 554, 14), // "onDisconnected"
QT_MOC_LITERAL(43, 569, 17), // "onConnectionError"
QT_MOC_LITERAL(44, 587, 8), // "errorMsg"
QT_MOC_LITERAL(45, 596, 23), // "onLoginResponseReceived"
QT_MOC_LITERAL(46, 620, 26), // "onRegisterResponseReceived"
QT_MOC_LITERAL(47, 647, 25), // "onSetUserResponseReceived"
QT_MOC_LITERAL(48, 673, 25), // "onGetUserResponseReceived"
QT_MOC_LITERAL(49, 699, 27), // "onGetAgendaResponseReceived"
QT_MOC_LITERAL(50, 727, 28), // "onGetContactResponseReceived"
QT_MOC_LITERAL(51, 756, 27), // "onGetDeviceResponseReceived"
QT_MOC_LITERAL(52, 784, 27), // "onAddDeviceResponseReceived"
QT_MOC_LITERAL(53, 812, 8), // "messages"
QT_MOC_LITERAL(54, 821, 30), // "onRemoveDeviceResponseReceived"
QT_MOC_LITERAL(55, 852, 27), // "onAddAgendaResponseReceived"
QT_MOC_LITERAL(56, 880, 33), // "onSendChatMessageResponseRece..."
QT_MOC_LITERAL(57, 914, 32), // "onGetChatHistoryResponseReceived"
QT_MOC_LITERAL(58, 947, 33), // "onAddSimulateDataResponseRece..."
QT_MOC_LITERAL(59, 981, 33), // "onNonContactUsersResponseRece..."
QT_MOC_LITERAL(60, 1015, 28), // "onAddContactResponseReceived"
QT_MOC_LITERAL(61, 1044, 31), // "onGetAllDevicesResponseReceived"
QT_MOC_LITERAL(62, 1076, 30), // "onUpdateDeviceResponseReceived"
QT_MOC_LITERAL(63, 1107, 24) // "onNewChatMessageReceived"

    },
    "Client\0connected\0\0disconnected\0"
    "connectionFailed\0messageReceived\0"
    "message\0loginResult\0success\0registerResult\0"
    "setUserResult\0User\0user\0getUserResult\0"
    "userData\0agendaResult\0QVector<Agenda>\0"
    "agendas\0contactResult\0QVector<User>\0"
    "contacts\0deviceResult\0QVector<Device>\0"
    "devices\0addDeviceResult\0removeDeviceResult\0"
    "addAgendaResult\0sendChatMessageResult\0"
    "getChatHistoryResult\0QVector<ChatMessage>\0"
    "history\0addSimulateDataResult\0"
    "nonContactUsersReceived\0QList<User>\0"
    "users\0addContactResult\0getAllDevicesResult\0"
    "updateDeviceResult\0newChatMessage\0"
    "contactId\0senderId\0onConnected\0"
    "onDisconnected\0onConnectionError\0"
    "errorMsg\0onLoginResponseReceived\0"
    "onRegisterResponseReceived\0"
    "onSetUserResponseReceived\0"
    "onGetUserResponseReceived\0"
    "onGetAgendaResponseReceived\0"
    "onGetContactResponseReceived\0"
    "onGetDeviceResponseReceived\0"
    "onAddDeviceResponseReceived\0messages\0"
    "onRemoveDeviceResponseReceived\0"
    "onAddAgendaResponseReceived\0"
    "onSendChatMessageResponseReceived\0"
    "onGetChatHistoryResponseReceived\0"
    "onAddSimulateDataResponseReceived\0"
    "onNonContactUsersResponseReceived\0"
    "onAddContactResponseReceived\0"
    "onGetAllDevicesResponseReceived\0"
    "onUpdateDeviceResponseReceived\0"
    "onNewChatMessageReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Client[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      43,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      22,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  229,    2, 0x06 /* Public */,
       3,    0,  230,    2, 0x06 /* Public */,
       4,    0,  231,    2, 0x06 /* Public */,
       5,    1,  232,    2, 0x06 /* Public */,
       7,    2,  235,    2, 0x06 /* Public */,
       9,    2,  240,    2, 0x06 /* Public */,
      10,    3,  245,    2, 0x06 /* Public */,
      13,    3,  252,    2, 0x06 /* Public */,
      15,    3,  259,    2, 0x06 /* Public */,
      18,    3,  266,    2, 0x06 /* Public */,
      21,    3,  273,    2, 0x06 /* Public */,
      24,    2,  280,    2, 0x06 /* Public */,
      25,    2,  285,    2, 0x06 /* Public */,
      26,    2,  290,    2, 0x06 /* Public */,
      27,    2,  295,    2, 0x06 /* Public */,
      28,    3,  300,    2, 0x06 /* Public */,
      31,    2,  307,    2, 0x06 /* Public */,
      32,    1,  312,    2, 0x06 /* Public */,
      35,    2,  315,    2, 0x06 /* Public */,
      36,    3,  320,    2, 0x06 /* Public */,
      37,    2,  327,    2, 0x06 /* Public */,
      38,    2,  332,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      41,    0,  337,    2, 0x08 /* Private */,
      42,    0,  338,    2, 0x08 /* Private */,
      43,    1,  339,    2, 0x08 /* Private */,
      45,    2,  342,    2, 0x08 /* Private */,
      46,    2,  347,    2, 0x08 /* Private */,
      47,    3,  352,    2, 0x08 /* Private */,
      48,    3,  359,    2, 0x08 /* Private */,
      49,    3,  366,    2, 0x08 /* Private */,
      50,    3,  373,    2, 0x08 /* Private */,
      51,    3,  380,    2, 0x08 /* Private */,
      52,    2,  387,    2, 0x08 /* Private */,
      54,    2,  392,    2, 0x08 /* Private */,
      55,    2,  397,    2, 0x08 /* Private */,
      56,    2,  402,    2, 0x08 /* Private */,
      57,    3,  407,    2, 0x08 /* Private */,
      58,    2,  414,    2, 0x08 /* Private */,
      59,    1,  419,    2, 0x08 /* Private */,
      60,    2,  422,    2, 0x08 /* Private */,
      61,    3,  427,    2, 0x08 /* Private */,
      62,    2,  434,    2, 0x08 /* Private */,
      63,    2,  439,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    8,    6,   12,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    8,    6,   14,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 16, QMetaType::QString,    8,   17,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 19, QMetaType::QString,    8,   20,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 22, QMetaType::QString,    8,   23,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 29, QMetaType::QString,    8,   30,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 22,    8,    6,   23,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   39,   40,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   44,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    8,    6,   12,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    8,    6,   14,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 16, QMetaType::QString,    8,   17,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 19, QMetaType::QString,    8,   20,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 22, QMetaType::QString,    8,   23,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,   53,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,   53,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 29, QMetaType::QString,    8,   23,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 22,    8,    6,   23,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    8,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   39,   40,

       0        // eod
};

void Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Client *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connectionFailed(); break;
        case 3: _t->messageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->loginResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->registerResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->setUserResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< User(*)>(_a[3]))); break;
        case 7: _t->getUserResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const User(*)>(_a[3]))); break;
        case 8: _t->agendaResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Agenda>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: _t->contactResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<User>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->deviceResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Device>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 11: _t->addDeviceResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->removeDeviceResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->addAgendaResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: _t->sendChatMessageResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 15: _t->getChatHistoryResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<ChatMessage>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 16: _t->addSimulateDataResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 17: _t->nonContactUsersReceived((*reinterpret_cast< const QList<User>(*)>(_a[1]))); break;
        case 18: _t->addContactResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: _t->getAllDevicesResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVector<Device>(*)>(_a[3]))); break;
        case 20: _t->updateDeviceResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->newChatMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->onConnected(); break;
        case 23: _t->onDisconnected(); break;
        case 24: _t->onConnectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->onLoginResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 26: _t->onRegisterResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 27: _t->onSetUserResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const User(*)>(_a[3]))); break;
        case 28: _t->onGetUserResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const User(*)>(_a[3]))); break;
        case 29: _t->onGetAgendaResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Agenda>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 30: _t->onGetContactResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<User>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 31: _t->onGetDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<Device>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 32: _t->onAddDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 33: _t->onRemoveDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 34: _t->onAddAgendaResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 35: _t->onSendChatMessageResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 36: _t->onGetChatHistoryResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QVector<ChatMessage>(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 37: _t->onAddSimulateDataResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 38: _t->onNonContactUsersResponseReceived((*reinterpret_cast< const QList<User>(*)>(_a[1]))); break;
        case 39: _t->onAddContactResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 40: _t->onGetAllDevicesResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVector<Device>(*)>(_a[3]))); break;
        case 41: _t->onUpdateDeviceResponseReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 42: _t->onNewChatMessageReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
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
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< User >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Agenda> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<User> >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<User> >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 27:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< User >(); break;
            }
            break;
        case 28:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< User >(); break;
            }
            break;
        case 29:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Agenda> >(); break;
            }
            break;
        case 30:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<User> >(); break;
            }
            break;
        case 31:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        case 38:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<User> >(); break;
            }
            break;
        case 40:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<Device> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Client::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Client::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Client::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::connectionFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Client::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::messageReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::loginResult)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::registerResult)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & , User );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::setUserResult)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & , const User & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::getUserResult)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QVector<Agenda> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::agendaResult)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QVector<User> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::contactResult)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QVector<Device> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::deviceResult)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::addDeviceResult)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::removeDeviceResult)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::addAgendaResult)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::sendChatMessageResult)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QVector<ChatMessage> & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::getChatHistoryResult)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::addSimulateDataResult)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (Client::*)(const QList<User> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::nonContactUsersReceived)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::addContactResult)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & , const QVector<Device> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::getAllDevicesResult)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (Client::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::updateDeviceResult)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (Client::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Client::newChatMessage)) {
                *result = 21;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Client::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Client.data,
    qt_meta_data_Client,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Client.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    }
    return _id;
}

// SIGNAL 0
void Client::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Client::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Client::connectionFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Client::messageReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Client::loginResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Client::registerResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Client::setUserResult(bool _t1, const QString & _t2, User _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Client::getUserResult(bool _t1, const QString & _t2, const User & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Client::agendaResult(bool _t1, const QVector<Agenda> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Client::contactResult(bool _t1, const QVector<User> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Client::deviceResult(bool _t1, const QVector<Device> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Client::addDeviceResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Client::removeDeviceResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Client::addAgendaResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Client::sendChatMessageResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Client::getChatHistoryResult(bool _t1, const QVector<ChatMessage> & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Client::addSimulateDataResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Client::nonContactUsersReceived(const QList<User> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void Client::addContactResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void Client::getAllDevicesResult(bool _t1, const QString & _t2, const QVector<Device> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void Client::updateDeviceResult(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void Client::newChatMessage(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
