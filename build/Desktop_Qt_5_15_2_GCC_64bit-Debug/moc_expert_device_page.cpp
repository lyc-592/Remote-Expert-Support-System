/****************************************************************************
** Meta object code from reading C++ file 'expert_device_page.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ui/index/expert/expert_device_page.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expert_device_page.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ExpertDevicePage_t {
    QByteArrayData data[14];
    char stringdata0[184];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExpertDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExpertDevicePage_t qt_meta_stringdata_ExpertDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ExpertDevicePage"
QT_MOC_LITERAL(1, 17, 20), // "refreshDevicesSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 18), // "updateDeviceSignal"
QT_MOC_LITERAL(4, 58, 10), // "deviceName"
QT_MOC_LITERAL(5, 69, 4), // "type"
QT_MOC_LITERAL(6, 74, 6), // "status"
QT_MOC_LITERAL(7, 81, 22), // "onRefreshButtonClicked"
QT_MOC_LITERAL(8, 104, 23), // "onGetAllDevicesReceived"
QT_MOC_LITERAL(9, 128, 7), // "success"
QT_MOC_LITERAL(10, 136, 7), // "message"
QT_MOC_LITERAL(11, 144, 15), // "QVector<Device>"
QT_MOC_LITERAL(12, 160, 7), // "devices"
QT_MOC_LITERAL(13, 168, 15) // "onControlDevice"

    },
    "ExpertDevicePage\0refreshDevicesSignal\0"
    "\0updateDeviceSignal\0deviceName\0type\0"
    "status\0onRefreshButtonClicked\0"
    "onGetAllDevicesReceived\0success\0message\0"
    "QVector<Device>\0devices\0onControlDevice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExpertDevicePage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    3,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   47,    2, 0x08 /* Private */,
       8,    3,   48,    2, 0x08 /* Private */,
      13,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, 0x80000000 | 11,    9,   10,   12,
    QMetaType::Void,

       0        // eod
};

void ExpertDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExpertDevicePage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refreshDevicesSignal(); break;
        case 1: _t->updateDeviceSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: _t->onRefreshButtonClicked(); break;
        case 3: _t->onGetAllDevicesReceived((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVector<Device>(*)>(_a[3]))); break;
        case 4: _t->onControlDevice(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
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
            using _t = void (ExpertDevicePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpertDevicePage::refreshDevicesSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ExpertDevicePage::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpertDevicePage::updateDeviceSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ExpertDevicePage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ExpertDevicePage.data,
    qt_meta_data_ExpertDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ExpertDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExpertDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExpertDevicePage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ExpertDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ExpertDevicePage::refreshDevicesSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ExpertDevicePage::updateDeviceSignal(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
