#ifndef CONFERENCE_UI_H
#define CONFERENCE_UI_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "../../client/core/client.h"
#include "../../server/core/server.h"
#include "message.h"

class ConferenceUi : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConferenceUi(QWidget *parent = nullptr);
    ~ConferenceUi();

private slots:
    void on_startServerButton_clicked();
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void on_attachButton_clicked();

    void onClientConnected();
    void onClientDisconnected();
    void onMessageReceived(const QByteArray &message);

    void onServerClientConnected();
    void onServerClientDisconnected();
    void onServerNewMessage(const QByteArray &message);

private:
    // UI 控件
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QGroupBox *serverGroup;
    QHBoxLayout *serverLayout;
    QSpinBox *serverPortSpinBox;
    QPushButton *startServerButton;

    QGroupBox *clientGroup;
    QHBoxLayout *clientLayout;
    QLineEdit *hostLineEdit;
    QSpinBox *clientPortSpinBox;
    QPushButton *connectButton;

    QGroupBox *messageGroup;
    QVBoxLayout *messageLayout;
    QTextEdit *messageTextEdit;
    QHBoxLayout *attachmentLayout;
    QPushButton *attachButton;
    QLabel *attachmentLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *sendButton;

    QGroupBox *chatGroup;
    QVBoxLayout *chatLayout;
    QTextEdit *chatTextEdit;

    QGroupBox *logGroup;
    QVBoxLayout *logLayout;
    QTextEdit *logTextEdit;

    // 业务对象
    Server *m_server;
    Client *m_client;

    // 附件相关
    QString m_attachmentPath;
    MessageType m_attachmentType;

    void displayMessage(const Message &message);
    void addLogMessage(const QString &message);
    void setupUI();
    void clearAttachment();
};

#endif // CONFERENCE_UI_H
