QT       += core gui \
            network \
            sql \
            core gui sql charts


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
    ui/index/expert/expert_history_order.cpp \
    ui/index/expert/expert_order_detail.cpp \
    ui/index/expert/expert_record.cpp \
    ui/index/expert/expertmainwindow.cpp \
    ui/index/factory/addagendadialog.cpp \
    ui/index/factory/agendapage.cpp \
    ui/index/factory/cardwidget.cpp \
    ui/index/factory/chatdialog.cpp \
    ui/index/factory/devicepage.cpp \
    ui/index/factory/history_order.cpp \
    ui/index/factory/homepage.cpp \
    ui/index/factory/mainwindow.cpp \
    ui/index/factory/order_detail_dialog.cpp \
    ui/index/factory/record_dialog.cpp \
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
    ui/index/expert/expert_history_order.h \
    ui/index/expert/expert_order_detail.h \
    ui/index/expert/expert_record.h \
    ui/index/expert/expertmainwindow.h \
    ui/index/factory/addagendadialog.h \
    ui/index/factory/agendapage.h \
    ui/index/factory/cardwidget.h \
    ui/index/factory/chatdialog.h \
    ui/index/factory/devicepage.h \
    ui/index/factory/history_order.h \
    ui/index/factory/homepage.h \
    ui/index/factory/mainwindow.h \
    ui/index/factory/order_detail_dialog.h \
    ui/index/factory/record_dialog.h \
    ui/login/find_password_dialog.h \
    ui/login/login_window.h \
    ui/login/register_dialog.h \
    utils/constants.h \
    utils/funcs.h \
    utils/network_interface_constants.h

FORMS += \
    ui/conference/conference_ui.ui \
    ui/index/factory/addagendadialog.ui \
    ui/index/factory/cardwidget.ui \
    ui/index/factory/mainwindow.ui \
    ui/login/login_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/test_css.qrc
