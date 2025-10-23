QT       += core gui \
            network \
            sql \
            charts \
            multimedia \
            multimediawidgets

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
    ui/conference/conference_ui.cpp \
    ui/conference/conferencewindow.cpp \
    ui/conference/mainconferencewindow.cpp \
    ui/conference/message.cpp \
    ui/conference/videothread.cpp \
    ui/conference/audiothread.cpp \
    ui/index/expert/expert_main_window.cpp \
    ui/index/factory/add_agenda_dialog.cpp \
    ui/index/factory/agenda_page.cpp \
    ui/index/factory/chat_dialog.cpp \
    ui/index/factory/device_page.cpp \
    ui/index/factory/main_window.cpp \
    ui/index/factory/user_page.cpp \
    ui/index/factory/history_order.cpp \
    ui/index/factory/record_dialog.cpp \
    ui/index/factory/order_detail_dialog.cpp \
    ui/index/factory/cardwidget.cpp \
    ui/index/factory/homepage.cpp \
    ui/index/expert/expert_history_order.cpp \
    ui/index/expert/expert_order_detail.cpp \
    ui/index/expert/expert_record.cpp \
    ui/index/expert/expert_agenda_page.cpp \
    ui/index/expert/expert_device_page.cpp \
    ui/knowledge/data_viewer.cpp \
    ui/login/find_password_dialog.cpp \
    ui/login/login_window.cpp \
    ui/login/register_dialog.cpp \
    main.cpp

HEADERS += \
    client/core/client.h \
    client/network/client_network_manager.h \
    server/db/database_manager.h \
    server/core/server.h \
    server/entities/agenda.h \
    server/entities/chat_message.h \
    server/entities/device.h \
    server/entities/user.h \
    test/client_server_test.h \
    test/conference_test.h \
    test/login_test.h \
    test/mainwindow_test.h \
    test/video_test.h \
    ui/conference/conference_ui.h \
    ui/conference/conferencewindow.h \
    ui/conference/mainconferencewindow.h \
    ui/conference/message.h\
    ui/conference/videothread.h \
    ui/conference/audiothread.h \
    ui/index/expert/expert_main_window.h \
    ui/index/expert/expert_agenda_page.h \
    ui/index/expert/expert_device_page.h \
    ui/index/factory/add_agenda_dialog.h \
    ui/index/factory/agenda_page.h \
    ui/index/factory/chat_dialog.h \
    ui/index/factory/device_page.h \
    ui/index/factory/main_window.h \
    ui/index/factory/user_page.h \
    ui/index/factory/history_order.h \
    ui/index/factory/record_dialog.h \
    ui/index/factory/order_detail_dialog.h \
    ui/index/factory/cardwidget.h \
    ui/index/factory/homepage.h \
    ui/index/expert/expert_history_order.h \
    ui/index/expert/expert_order_detail.h \
    ui/index/expert/expert_record.h \
    ui/knowledge/data_viewer.h \
    ui/login/find_password_dialog.h \
    ui/login/login_window.h \
    ui/login/register_dialog.h \
    utils/constants.h \
    utils/funcs.h \
    utils/network_interface_constants.h

FORMS += \
    ui/conference/conference_ui.ui \
    ui/conference/conferencewindow.ui \
    ui/index/factory/add_agenda_dialog.ui \
    ui/index/factory/main_window.ui \
    ui/login/login_window.ui \
    ui/index/factory/cardwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/test_css.qrc \
    src/resources.qrc
