/****************************************************************************
** Meta object code from reading C++ file 'conferencewindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ui/conference/conferencewindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conferencewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConferenceWindow_t {
    QByteArrayData data[26];
    char stringdata0[444];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceWindow_t qt_meta_stringdata_ConferenceWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ConferenceWindow"
QT_MOC_LITERAL(1, 17, 25), // "init_ConferenceWindowInfo"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 17), // "init_CameraDevice"
QT_MOC_LITERAL(4, 62, 11), // "QCameraInfo"
QT_MOC_LITERAL(5, 74, 6), // "device"
QT_MOC_LITERAL(6, 81, 21), // "init_CameraDeviceInfo"
QT_MOC_LITERAL(7, 103, 23), // "handle_ShowCameraDevice"
QT_MOC_LITERAL(8, 127, 5), // "index"
QT_MOC_LITERAL(9, 133, 25), // "on_startServerBtn_clicked"
QT_MOC_LITERAL(10, 159, 21), // "on_connectBtn_clicked"
QT_MOC_LITERAL(11, 181, 14), // "handle_Connect"
QT_MOC_LITERAL(12, 196, 15), // "init_TimerModel"
QT_MOC_LITERAL(13, 212, 19), // "handle_InterceptImg"
QT_MOC_LITERAL(14, 232, 16), // "on_imageCaptured"
QT_MOC_LITERAL(15, 249, 2), // "id"
QT_MOC_LITERAL(16, 252, 5), // "image"
QT_MOC_LITERAL(17, 258, 14), // "init_TcpModule"
QT_MOC_LITERAL(18, 273, 18), // "init_TcpConnection"
QT_MOC_LITERAL(19, 292, 20), // "hanlde_RelayinThread"
QT_MOC_LITERAL(20, 313, 13), // "get_LocalIPv4"
QT_MOC_LITERAL(21, 327, 26), // "on_toggleCameraBtn_clicked"
QT_MOC_LITERAL(22, 354, 23), // "on_toggleMicBtn_clicked"
QT_MOC_LITERAL(23, 378, 26), // "on_recordScreenBtn_clicked"
QT_MOC_LITERAL(24, 405, 17), // "on_audioDataReady"
QT_MOC_LITERAL(25, 423, 20) // "on_audioDataReceived"

    },
    "ConferenceWindow\0init_ConferenceWindowInfo\0"
    "\0init_CameraDevice\0QCameraInfo\0device\0"
    "init_CameraDeviceInfo\0handle_ShowCameraDevice\0"
    "index\0on_startServerBtn_clicked\0"
    "on_connectBtn_clicked\0handle_Connect\0"
    "init_TimerModel\0handle_InterceptImg\0"
    "on_imageCaptured\0id\0image\0init_TcpModule\0"
    "init_TcpConnection\0hanlde_RelayinThread\0"
    "get_LocalIPv4\0on_toggleCameraBtn_clicked\0"
    "on_toggleMicBtn_clicked\0"
    "on_recordScreenBtn_clicked\0on_audioDataReady\0"
    "on_audioDataReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x08 /* Private */,
       3,    1,  110,    2, 0x08 /* Private */,
       6,    0,  113,    2, 0x08 /* Private */,
       7,    1,  114,    2, 0x08 /* Private */,
       9,    0,  117,    2, 0x08 /* Private */,
      10,    0,  118,    2, 0x08 /* Private */,
      11,    0,  119,    2, 0x08 /* Private */,
      12,    0,  120,    2, 0x08 /* Private */,
      13,    0,  121,    2, 0x08 /* Private */,
      14,    2,  122,    2, 0x08 /* Private */,
      17,    0,  127,    2, 0x08 /* Private */,
      18,    0,  128,    2, 0x08 /* Private */,
      19,    0,  129,    2, 0x08 /* Private */,
      20,    0,  130,    2, 0x08 /* Private */,
      21,    0,  131,    2, 0x08 /* Private */,
      22,    0,  132,    2, 0x08 /* Private */,
      23,    0,  133,    2, 0x08 /* Private */,
      24,    0,  134,    2, 0x08 /* Private */,
      25,    0,  135,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConferenceWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConferenceWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init_ConferenceWindowInfo(); break;
        case 1: _t->init_CameraDevice((*reinterpret_cast< const QCameraInfo(*)>(_a[1]))); break;
        case 2: _t->init_CameraDeviceInfo(); break;
        case 3: _t->handle_ShowCameraDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_startServerBtn_clicked(); break;
        case 5: _t->on_connectBtn_clicked(); break;
        case 6: _t->handle_Connect(); break;
        case 7: _t->init_TimerModel(); break;
        case 8: _t->handle_InterceptImg(); break;
        case 9: _t->on_imageCaptured((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        case 10: _t->init_TcpModule(); break;
        case 11: _t->init_TcpConnection(); break;
        case 12: _t->hanlde_RelayinThread(); break;
        case 13: { int _r = _t->get_LocalIPv4();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->on_toggleCameraBtn_clicked(); break;
        case 15: _t->on_toggleMicBtn_clicked(); break;
        case 16: _t->on_recordScreenBtn_clicked(); break;
        case 17: _t->on_audioDataReady(); break;
        case 18: _t->on_audioDataReceived(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCameraInfo >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConferenceWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ConferenceWindow.data,
    qt_meta_data_ConferenceWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConferenceWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ConferenceWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
