#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../../server/entities/chat_message.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(const QString &contactName, const int userId, QWidget *parent = nullptr);
    ~ChatDialog();
    int getContactId() const {
        return contactId;
    }
    void updateChatHistory(const QVector<ChatMessage>& history);

private slots:
    void onSendClicked();

signals:
    void sendMessageSignal(const int contactId, const QString& message);

private:
    void setupUI();
    void addSystemMessage(const QString &message);
    void addMyMessage(const QString &message, QString &time);
    void addTheirMessage(const QString &message, QString &time);
    void scrollToBottom();

    QString contactName;
    int contactId;
    QLabel *titleLabel;
    QTextEdit *chatView;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QVector<ChatMessage> history;
};

#endif // CHAT_DIALOG_H
