#include "chat_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QFont>

ChatDialog::ChatDialog(const QString &contactName, QWidget *parent)
    : QDialog(parent), contactName(contactName)
{
    setupUI();
    setWindowTitle("与 " + contactName + " 聊天");
    setMinimumSize(450, 550);
    setStyleSheet("QDialog { background: #f0f2f5; }");
}

void ChatDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 聊天显示区域
    chatView = new QTextEdit;
    chatView->setObjectName("ChatView");
    chatView->setReadOnly(true);
    chatView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chatView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 输入区域
    QWidget *inputWidget = new QWidget;
    inputWidget->setObjectName("InputWidget");
    inputWidget->setFixedHeight(60);

    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(15, 10, 15, 10);
    inputLayout->setSpacing(10);

    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("输入消息...");
    messageInput->setObjectName("MessageInput");

    sendButton = new QPushButton("发送");
    sendButton->setObjectName("SendButton");
    sendButton->setFixedSize(60, 35);

    inputLayout->addWidget(messageInput, 1);
    inputLayout->addWidget(sendButton);

    // 添加到主布局
    mainLayout->addWidget(chatView, 1);
    mainLayout->addWidget(inputWidget);

    // 连接信号
    connect(sendButton, &QPushButton::clicked, this, &ChatDialog::onSendClicked);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatDialog::onSendClicked);

    // 设置聊天视图样式
    chatView->setStyleSheet(R"(
        QTextEdit {
            background: #f8f9fa;
            border: none;
            padding: 10px;
            font-size: 14px;
        }
    )");

}

void ChatDialog::addMyMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm");

    // 计算消息的大致长度
    int charCount = 0;
    for (const QChar &c : message) {
        charCount += (c.unicode() > 255) ? 2 : 1;
    }

    // 根据消息长度调整最大宽度
    int maxWidthPercent = 70;
    if (charCount < 10) {
        maxWidthPercent = 40;
    } else if (charCount < 20) {
        maxWidthPercent = 60;
    }

    // 使用表格实现精确右对齐
    QString html = QString("<table width='100%' cellpadding='0' cellspacing='0' border='0' style='margin: 8px 0;'>"
                           "<tr>"
                           "<td align='right' valign='top'>"
                           "<table cellpadding='0' cellspacing='0' border='0' style='display: inline-table; max-width: %1%;'>"
                           "<tr>"
                           "<td style='background: #d8d3f4; color: black; padding: 8px 12px; "
                           "border-radius: 18px 0 18px 18px; word-wrap: break-word; font-size: 14px;'>"
                           "%2"
                           "</td>"
                           "</tr>"
                           "<tr>"
                           "<td align='right' style='padding: 2px 8px 0 0;'>"
                           "<span style='color: #999; font-size: 11px;'>%3</span>"
                           "</td>"
                           "</tr>"
                           "</table>"
                           "</td>"
                           "</tr>"
                           "</table>").arg(maxWidthPercent).arg(message.toHtmlEscaped()).arg(timestamp);

    chatView->append(html);
    scrollToBottom();
}

void ChatDialog::addTheirMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm");

    // 计算消息的大致长度
    int charCount = 0;
    for (const QChar &c : message) {
        charCount += (c.unicode() > 255) ? 2 : 1;
    }

    // 根据消息长度调整最大宽度
    int maxWidthPercent = 70;
    if (charCount < 10) {
        maxWidthPercent = 40;
    } else if (charCount < 20) {
        maxWidthPercent = 60;
    }

    // 使用表格实现精确左对齐
    QString html = QString("<table width='100%' cellpadding='0' cellspacing='0' border='0' style='margin: 8px 0;'>"
                           "<tr>"
                           "<td align='left' valign='top'>"
                           "<table cellpadding='0' cellspacing='0' border='0' style='display: inline-table; max-width: %1%;'>"
                           "<tr>"
                           "<td style='background: white; color: #333; padding: 8px 12px; "
                           "border-radius: 0 18px 18px 18px; word-wrap: break-word; font-size: 14px; "
                           "box-shadow: 0 1px 2px rgba(0,0,0,0.1);'>"
                           "%2"
                           "</td>"
                           "</tr>"
                           "<tr>"
                           "<td align='left' style='padding: 2px 0 0 8px;'>"
                           "<span style='color: #999; font-size: 11px;'>%3</span>"
                           "</td>"
                           "</tr>"
                           "</table>"
                           "</td>"
                           "</tr>"
                           "</table>").arg(maxWidthPercent).arg(message.toHtmlEscaped()).arg(timestamp);

    chatView->append(html);
    scrollToBottom();
}

void ChatDialog::scrollToBottom()
{
    QTimer::singleShot(0, this, [this]() {
        QScrollBar *scrollbar = chatView->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    });
}

void ChatDialog::onSendClicked()
{
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) return;

    // 显示我发送的消息（右侧气泡）
    addMyMessage(message);

    // 模拟对方回复（左侧气泡）
    QTimer::singleShot(1000 + (rand() % 2000), this, [this, message]() {
        // 模拟不同类型的回复
        QString reply;
        if (message.contains("?")) {
            reply = "这个问题我需要查一下资料，稍后回复您。";
        } else if (message.length() < 5) {
            reply = "收到！";
        } else {
            reply = QString("好的，关于\"%1\"，我会尽快处理。").arg(message.left(20));
        }

        addTheirMessage(reply);
    });

    messageInput->clear();
    messageInput->setFocus();
}
ChatDialog::~ChatDialog()
{
}
