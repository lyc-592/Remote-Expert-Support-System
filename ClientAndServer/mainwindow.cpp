#include "mainwindow.h"
#include "server.h"
#include "client.h"
#include <QFileDialog>
#include <QDateTime>
#include <QImageReader>
#include <QMessageBox>
#include <QTextCursor>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_server(new Server()),
    m_client(new Client("test_user", "1", "123")),
    m_attachmentPath(""),
    m_attachmentType(TextMessage)
{
    setupUI();

    // 连接服务器信号
    connect(m_server, &Server::clientConnected, this, &MainWindow::onServerClientConnected);
    connect(m_server, &Server::clientDisconnected, this, &MainWindow::onServerClientDisconnected);
    connect(m_server, &Server::newMessage, this, &MainWindow::onServerNewMessage);

    // 连接客户端信号
    connect(m_client, &Client::connected, this, &MainWindow::onClientConnected);
    connect(m_client, &Client::disconnected, this, &MainWindow::onClientDisconnected);
    connect(m_client, &Client::messageReceived, this, &MainWindow::onMessageReceived);
}

MainWindow::~MainWindow()
{
    // UI 控件会自动被父对象删除
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // 服务器组
    serverGroup = new QGroupBox("Server", centralWidget);
    serverLayout = new QHBoxLayout(serverGroup);

    serverLayout->addWidget(new QLabel("Port:", serverGroup));
    serverPortSpinBox = new QSpinBox(serverGroup);
    serverPortSpinBox->setRange(1024, 65535);
    serverPortSpinBox->setValue(12345);
    serverLayout->addWidget(serverPortSpinBox);

    startServerButton = new QPushButton("Start Server", serverGroup);
    serverLayout->addWidget(startServerButton);

    mainLayout->addWidget(serverGroup);

    // 客户端组
    clientGroup = new QGroupBox("Client", centralWidget);
    clientLayout = new QHBoxLayout(clientGroup);

    clientLayout->addWidget(new QLabel("Host:", clientGroup));
    hostLineEdit = new QLineEdit("localhost", clientGroup);
    clientLayout->addWidget(hostLineEdit);

    clientLayout->addWidget(new QLabel("Port:", clientGroup));
    clientPortSpinBox = new QSpinBox(clientGroup);
    clientPortSpinBox->setRange(1024, 65535);
    clientPortSpinBox->setValue(12345);
    clientLayout->addWidget(clientPortSpinBox);

    connectButton = new QPushButton("Connect", clientGroup);
    clientLayout->addWidget(connectButton);

    mainLayout->addWidget(clientGroup);

    // 消息发送组
    messageGroup = new QGroupBox("Send Message", centralWidget);
    messageLayout = new QVBoxLayout(messageGroup);

    messageTextEdit = new QTextEdit(messageGroup);
    messageLayout->addWidget(messageTextEdit);

    // 附件布局
    attachmentLayout = new QHBoxLayout();
    attachButton = new QPushButton("Attach File/Image", messageGroup);
    attachmentLabel = new QLabel("No attachment", messageGroup);
    attachmentLabel->setWordWrap(true);

    attachmentLayout->addWidget(attachButton);
    attachmentLayout->addWidget(attachmentLabel, 1); // 让标签占据剩余空间

    messageLayout->addLayout(attachmentLayout);

    // 发送按钮
    buttonLayout = new QHBoxLayout();
    sendButton = new QPushButton("Send", messageGroup);
    buttonLayout->addWidget(sendButton);

    messageLayout->addLayout(buttonLayout);

    mainLayout->addWidget(messageGroup);

    // 聊天显示组
    chatGroup = new QGroupBox("Chat", centralWidget);
    chatLayout = new QVBoxLayout(chatGroup);

    chatTextEdit = new QTextEdit(chatGroup);
    chatTextEdit->setReadOnly(true);
    chatLayout->addWidget(chatTextEdit);

    mainLayout->addWidget(chatGroup);

    // 日志组
    logGroup = new QGroupBox("Log", centralWidget);
    logLayout = new QVBoxLayout(logGroup);

    logTextEdit = new QTextEdit(logGroup);
    logTextEdit->setReadOnly(true);
    logLayout->addWidget(logTextEdit);

    mainLayout->addWidget(logGroup);

    // 连接按钮信号
    connect(startServerButton, &QPushButton::clicked, this, &MainWindow::on_startServerButton_clicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);
    connect(attachButton, &QPushButton::clicked, this, &MainWindow::on_attachButton_clicked);

    setWindowTitle("Real Time Communication");
    resize(800, 600);
}

void MainWindow::on_startServerButton_clicked()
{
    quint16 port = serverPortSpinBox->value();
    if (m_server->startServer(port)) {
        addLogMessage("Server started on port " + QString::number(port));
    } else {
        addLogMessage("Failed to start server: " + m_server->errorString());
    }
}

void MainWindow::on_connectButton_clicked()
{
    QString host = hostLineEdit->text();
    quint16 port = clientPortSpinBox->value();

    m_client->connectToServer(host, port);
    addLogMessage("Connecting to " + host + ":" + QString::number(port));
}

void MainWindow::on_attachButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File or Image");
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

void MainWindow::on_sendButton_clicked()
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
    } else {
        addLogMessage("Not connected. Cannot send message.");
        return;
    }

    // 显示自己发送的消息
    // displayMessage(message);
}

void MainWindow::clearAttachment()
{
    m_attachmentPath = "";
    m_attachmentType = TextMessage;
    attachmentLabel->setText("No attachment");
}

void MainWindow::onClientConnected()
{
    addLogMessage("Connected to server");
}

void MainWindow::onClientDisconnected()
{
    addLogMessage("Disconnected from server");
}

void MainWindow::onMessageReceived(const QByteArray &message)
{
    Message msg = Message::fromByteArray(message);
    displayMessage(msg);
}

void MainWindow::onServerClientConnected()
{
    addLogMessage("Client connected to server");
}

void MainWindow::onServerClientDisconnected()
{
    addLogMessage("Client disconnected from server");
}

void MainWindow::onServerNewMessage(const QByteArray &message)
{
    Message msg = Message::fromByteArray(message);
    displayMessage(msg);
}

void MainWindow::displayMessage(const Message &message)
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
            chatTextEdit->append("[" + time + "] " + senderPrefix + "Received image: ");
            QTextCursor cursor = chatTextEdit->textCursor();
            cursor.insertImage(image.scaled(200, 200, Qt::KeepAspectRatio));
        }
        break;
    }
    case FileMessage:
        chatTextEdit->append("[" + time + "] " + senderPrefix + "Received file: " + message.fileName() +
                             " (" + QString::number(message.fileSize() / 1024) + " KB)");
        break;
    }
}

void MainWindow::addLogMessage(const QString &message)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append("[" + time + "] " + message);
}
