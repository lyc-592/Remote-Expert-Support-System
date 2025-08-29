QT       += core gui \
            network \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client/core/client.cpp \
    client/network/client_network_manager.cpp \
    server/core/server.cpp \
    server/db/database_manager.cpp \
    test.cpp \
    ui/conference/conference_ui.cpp \
    ui/conference/message.cpp \
    ui/index/expert/expertmainwindow.cpp \
    ui/index/factory/addagendadialog.cpp \
    ui/index/factory/agendapage.cpp \
    ui/index/factory/devicepage.cpp \
    ui/index/factory/mainwindow.cpp \
    ui/login/find_password_dialog.cpp \
    ui/login/login_window.cpp \
    ui/login/register_dialog.cpp \
    main.cpp

HEADERS += \
    client/core/client.h \
    client/network/client_network_manager.h \
    server/db/database_manager.h \
    server/core/server.h \
    test/client_server_test.h \
    test/conference_test.h \
    ui/conference/conference_ui.h \
    ui/conference/message.h\
    ui/index/expert/expertmainwindow.h \
    ui/index/factory/addagendadialog.h \
    ui/index/factory/agendapage.h \
    ui/index/factory/devicepage.h \
    ui/index/factory/mainwindow.h \
    ui/login/find_password_dialog.h \
    ui/login/login_window.h \
    ui/login/register_dialog.h \
    utils/constants.h \
    utils/funcs.h \
    utils/network_interface_constants.h

FORMS += \
    ui/conference/conference_ui.ui \
    ui/index/factory/addagendadialog.ui \
    ui/index/factory/mainwindow.ui \
    ui/login/login_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/test_css.qrc \
    src/resources.qrc
