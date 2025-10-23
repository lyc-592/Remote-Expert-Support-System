#include "conference_ui.h"
#include "buttonobjecthandler.h"
#include "../../utils/network_interface_constants.h"
#include <QFileDialog>
#include <QDateTime>
#include <QImageReader>
#include <QMessageBox>
#include <QTextCursor>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDebug>
#include <QTextFrame>
#include <QTextTableCell>
#include <QAbstractTextDocumentLayout>
#include <QAbstractScrollArea>
#include <QTextBrowser>

ConferenceUi::ConferenceUi(QWidget *parent) :
    QMainWindow(parent),
    m_server(new Server()),
    m_client(new Client()),
    m_attachmentPath(""),
    m_attachmentType(TextMessage)
{
    setupUI();

    // 连接服务器信号
    connect(m_server, &Server::clientConnected, this, &ConferenceUi::onServerClientConnected);
    connect(m_server, &Server::clientDisconnected, this, &ConferenceUi::onServerClientDisconnected);
    connect(m_server, &Server::newMessage, this, &ConferenceUi::onServerNewMessage);

    // 连接客户端信号
    connect(m_client, &Client::connected, this, &ConferenceUi::onClientConnected);
    connect(m_client, &Client::disconnected, this, &ConferenceUi::onClientDisconnected);
    connect(m_client, &Client::messageReceived, this, &ConferenceUi::onMessageReceived);
}

ConferenceUi::~ConferenceUi()
{
    // UI 控件会自动被父对象删除
}

void ConferenceUi::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // 服务器组
    serverGroup = new QGroupBox("服务器", centralWidget);
    serverLayout = new QHBoxLayout(serverGroup);

    serverLayout->addWidget(new QLabel("端口", serverGroup));
    serverPortSpinBox = new QSpinBox(serverGroup);
    serverPortSpinBox->setRange(1024, 65535);
    serverPortSpinBox->setValue(12345);
    serverLayout->addWidget(serverPortSpinBox);

    startServerButton = new QPushButton("启动服务器", serverGroup);
    serverLayout->addWidget(startServerButton);

    mainLayout->addWidget(serverGroup);

    // 客户端组
    clientGroup = new QGroupBox("用户", centralWidget);
    clientLayout = new QHBoxLayout(clientGroup);

    clientLayout->addWidget(new QLabel("主机", clientGroup));
    hostLineEdit = new QLineEdit("localhost", clientGroup);
    clientLayout->addWidget(hostLineEdit);

    clientLayout->addWidget(new QLabel("端口", clientGroup));
    clientPortSpinBox = new QSpinBox(clientGroup);
    clientPortSpinBox->setRange(1024, 65535);
    clientPortSpinBox->setValue(12345);
    clientLayout->addWidget(clientPortSpinBox);

    connectButton = new QPushButton("连接", clientGroup);
    clientLayout->addWidget(connectButton);

    mainLayout->addWidget(clientGroup);

    // 消息发送组
    messageGroup = new QGroupBox("发送信息", centralWidget);
    messageLayout = new QVBoxLayout(messageGroup);

    messageTextEdit = new QTextEdit(messageGroup);
    messageLayout->addWidget(messageTextEdit);

    // 附件布局
    attachmentLayout = new QHBoxLayout();
    attachButton = new QPushButton("附件", messageGroup);
    attachmentLabel = new QLabel("无", messageGroup);
    attachmentLabel->setWordWrap(true);

    attachmentLayout->addWidget(attachButton);
    attachmentLayout->addWidget(attachmentLabel, 1); // 让标签占据剩余空间

    messageLayout->addLayout(attachmentLayout);

    // 发送按钮
    buttonLayout = new QHBoxLayout();
    sendButton = new QPushButton("发送", messageGroup);
    buttonLayout->addWidget(sendButton);

    messageLayout->addLayout(buttonLayout);

    mainLayout->addWidget(messageGroup);

    // 聊天显示组
    chatGroup = new QGroupBox("聊天区", centralWidget);
    chatLayout = new QVBoxLayout(chatGroup);

    chatTextEdit = new QTextBrowser(chatGroup);
    chatTextEdit->setReadOnly(true);
    chatTextEdit->setAcceptRichText(true);
    chatLayout->addWidget(chatTextEdit);

    mainLayout->addWidget(chatGroup);

    // 日志组
    logGroup = new QGroupBox("日志", centralWidget);
    logLayout = new QVBoxLayout(logGroup);

    logTextEdit = new QTextEdit(logGroup);
    logTextEdit->setReadOnly(true);
    logLayout->addWidget(logTextEdit);

    mainLayout->addWidget(logGroup);

    m_imageMap = QMap<QString, QByteArray>();
    m_fileMap = QMap<QString, QPair<QByteArray, QString>>();

    // 连接按钮信号
    connect(startServerButton, &QPushButton::clicked, this, &ConferenceUi::on_startServerButton_clicked);
    connect(connectButton, &QPushButton::clicked, this, &ConferenceUi::on_connectButton_clicked);
    connect(sendButton, &QPushButton::clicked, this, &ConferenceUi::on_sendButton_clicked);
    connect(attachButton, &QPushButton::clicked, this, &ConferenceUi::on_attachButton_clicked);
    connect(chatTextEdit, &QTextBrowser::anchorClicked, this, &ConferenceUi::onAnchorClicked);
}

void ConferenceUi::on_startServerButton_clicked()
{
    quint16 port = serverPortSpinBox->value();
    if (m_server->startServer(port)) {
        addLogMessage("启动服务器： " + QString::number(port));
    } else {
        addLogMessage("启动服务器失败 " + m_server->errorString());
    }
}

void ConferenceUi::on_connectButton_clicked()
{
    QString host = hostLineEdit->text();
    quint16 port = clientPortSpinBox->value();

    m_client->connectToServer(host, port);
    addLogMessage("连接" + host + ":" + QString::number(port));
}

void ConferenceUi::on_attachButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件或图片");
    if (fileName.isEmpty()) return;

    m_attachmentPath = fileName;

    // 确定文件类型
    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(fileName).name();

    if (mimeType.startsWith("image/")) {
        m_attachmentType = ImageMessage;
        attachmentLabel->setText("Image: " + QFileInfo(fileName).fileName());
    } else {
        m_attachmentType = FileMessage;
        attachmentLabel->setText("File: " + QFileInfo(fileName).fileName());
    }
}

void ConferenceUi::on_sendButton_clicked()
{
    Message message;

    // 检查是否有附件
    if (!m_attachmentPath.isEmpty()) {
        QFile file(m_attachmentPath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        QFileInfo fileInfo(m_attachmentPath);

        if (m_attachmentType == ImageMessage) {
            message = Message::createImageMessage(QImage::fromData(fileData), "PNG");
        } else {
            message = Message::createFileMessage(fileData, fileInfo.fileName());
        }

        clearAttachment();
    } else {
        // 发送文本消息
        QString text = messageTextEdit->toPlainText();
        if (text.isEmpty()) return;

        message = Message::createTextMessage(text);
        messageTextEdit->clear();
        qDebug() << "Send text.";
    }

    QByteArray data = message.toByteArray();

    if (m_server->isListening()) {
        // 作为服务器广播消息
        emit m_server->newMessage(data);
    } else if (m_client->isConnected()) {
        m_client->sendMessage(data);
        // qDebug() << "Message Sent: " << data;
    } else {
        addLogMessage("Not connected. Cannot send message.");
        return;
    }

    // 显示自己发送的消息
    displayMessage(message);
}

void ConferenceUi::clearAttachment()
{
    m_attachmentPath = "";
    m_attachmentType = TextMessage;
    attachmentLabel->setText("No attachment");
}

void ConferenceUi::onClientConnected()
{
    addLogMessage("Connected to server");
}

void ConferenceUi::onClientDisconnected()
{
    addLogMessage("Disconnected from server");
}

void ConferenceUi::onMessageReceived(const QByteArray &message)
{
    Message msg = Message::fromByteArray(message);
    displayMessage(msg);
}

void ConferenceUi::onServerClientConnected()
{
    addLogMessage("Client connected to server");
}

void ConferenceUi::onServerClientDisconnected()
{
    addLogMessage("Client disconnected from server");
}

void ConferenceUi::onServerNewMessage(const QByteArray &message)
{
    Message msg = Message::fromByteArray(message);
    displayMessage(msg);
}

void ConferenceUi::displayMessage(const Message &message)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString senderPrefix = "";

    // 判断消息来源（简化版，实际应用中应该包含更详细的信息）
    if (message.type() == TextMessage && message.text().startsWith("[")) {
        // 可能是其他客户端发送的消息
        senderPrefix = "[Remote] ";
    } else {
        senderPrefix = "[Local] ";
    }

    switch (message.type()) {
    case TextMessage:
        chatTextEdit->append("[" + time + "] " + senderPrefix + message.text());
        break;
    case ImageMessage:
    {
        QImage image = message.image();
        if (!image.isNull()) {
            chatTextEdit->append("[" + time + "] " + senderPrefix + "Received image: \n");
            QTextCursor cursor = chatTextEdit->textCursor();
            cursor.insertImage(image.scaled(200, 200, Qt::KeepAspectRatio));
            // 添加一个可点击的链接作为下载按钮
            QString downloadLink = QString("<a href=\"download://image/%1\" style=\"color: blue; text-decoration: underline;\">[下载图片]</a>").arg(time);
            chatTextEdit->append(downloadLink);

            // 存储图片数据以便下载
            m_imageMap[time] = message.data();
        }
        break;
    }
    case FileMessage:
    {
        chatTextEdit->append("[" + time + "] " + senderPrefix + "Received file: " + message.fileName() +
                             " (" + QString::number(message.fileSize() / 1024) + " KB)");
        // 添加一个可点击的链接作为下载按钮
        QString downloadLink = QString("<a href=\"download://file/%1\" style=\"color: blue; text-decoration: underline;\">[下载文件]</a>").arg(time);
        chatTextEdit->append(downloadLink);

        // 存储图片数据以便下载
        m_fileMap[time] = qMakePair(message.data(), message.fileName());
        break;
    }
    }
}

void ConferenceUi::addLogMessage(const QString &message)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append("[" + time + "] " + message);
}

void ConferenceUi::downloadImage(const QString &timestamp)
{
    if (!m_imageMap.contains(timestamp)) {
        QMessageBox::warning(this, "Error", "Image data not found");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "image.png", "PNG Images (*.png);;JPEG Images (*.jpg *.jpeg)");

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file for writing: " + file.errorString());
        return;
    }

    file.write(m_imageMap[timestamp]);
    file.close();

    QMessageBox::information(this, "Success", "Image saved successfully");
}

void ConferenceUi::downloadFile(const QString &timestamp)
{
    if (!m_fileMap.contains(timestamp)) {
        QMessageBox::warning(this, "Error", "File data not found");
        return;
    }

    QPair<QByteArray, QString> fileData = m_fileMap[timestamp];
    QString suggestedName = fileData.second;

    QString filePath = QFileDialog::getSaveFileName(this, "Save File", suggestedName, "All Files (*)");

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file for writing: " + file.errorString());
        return;
    }

    file.write(fileData.first);
    file.close();

    QMessageBox::information(this, "Success", "File saved successfully");
}

void ConferenceUi::onAnchorClicked(const QUrl &url)
{
    if (url.scheme() == "download") {
        QString type = url.host();
        QString timestamp = url.path().mid(1); // 移除前面的斜杠

        if (type == "image") {
            downloadImage(timestamp);
        } else if (type == "file") {
            downloadFile(timestamp);
        }
    }
}
