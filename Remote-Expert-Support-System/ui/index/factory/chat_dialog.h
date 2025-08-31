#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(const QString &contactName, QWidget *parent = nullptr);
    ~ChatDialog();

private slots:
    void onSendClicked();

private:
    void setupUI();
    void addSystemMessage(const QString &message);
    void addMyMessage(const QString &message);
    void addTheirMessage(const QString &message);
    void scrollToBottom();

    QString contactName;
    QLabel *titleLabel;
    QTextEdit *chatView;
    QLineEdit *messageInput;
    QPushButton *sendButton;
};

#endif // CHAT_DIALOG_H
