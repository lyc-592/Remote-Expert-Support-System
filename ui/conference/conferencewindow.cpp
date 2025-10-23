#include "conferencewindow.h"
#include "videothread.h"
#include "audiothread.h"  // 新增
#include "conference_ui.h"
#include <QApplication>
#include <QCameraInfo>
#include <QAudioDeviceInfo>
#include <QDir>
#include <QHostInfo>
#include <QDebug>
#include <QBuffer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QThreadPool>
#include <QNetworkInterface>
#include <QException>
#include <QStandardPaths> // 新增：用于获取标准路径
#include <QDateTime>     // 新增：用于生成时间戳
#include <QDesktopWidget> // 新增：获取屏幕尺寸


static const int qCameraInfoMetaTypeId = qRegisterMetaType<QCameraInfo>();

// ConferenceWindow::ConferenceWindow(QWidget *parent)
//     : QMainWindow(parent), camera_Obj(nullptr), myVideoPreview(nullptr),
//     capture_Obj(nullptr), audioInput(nullptr),
//     tcpSocket(nullptr), tcpServer(nullptr), timer_Obj(nullptr), video_ThreadObj(nullptr),
//     udpAudioSocket(nullptr),
//     audioOutput(nullptr),
//     audioInputDevice(nullptr),
//     audioOutputDevice(nullptr),
//     audioSenderThread(nullptr),
//     audioReceiverThread(nullptr),
//     audioEnabled(false),
//     remoteAudioReady(false),
//     ffmpegProcess(nullptr),
//     isRecording(false)
// {
//     // 创建主窗口控件
//     QWidget *centralWidget = new QWidget(this);
//     setCentralWidget(centralWidget);

//     // 创建布局
//     QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
//     QVBoxLayout *leftLayout = new QVBoxLayout();
//     QVBoxLayout *rightLayout = new QVBoxLayout();
//     QVBoxLayout *rrLayout = new QVBoxLayout();

//     // 左侧面板 - 本地视频和设置
//     QGroupBox *localGroup = new QGroupBox("本地设置");
//     QVBoxLayout *localLayout = new QVBoxLayout(localGroup);

//     // 使用QVideoWidget作为视频预览
//     myVideoPreview = new QVideoWidget();
//     myVideoPreview->setMinimumSize(320, 240);

//     comboCam_List = new QComboBox();
//     comboCam_VideoRes = new QComboBox();
//     comboCam_FrameRate = new QComboBox();

//     QHBoxLayout *cameraLayout = new QHBoxLayout();
//     toggleCameraBtn = new QPushButton("关闭摄像头");
//     toggleMicBtn = new QPushButton("开启麦克风");
//     recordScreenBtn = new QPushButton("开始录制屏幕"); // 新增：录制屏幕按钮
//     cameraLayout->addWidget(toggleCameraBtn);
//     cameraLayout->addWidget(toggleMicBtn);
//     cameraLayout->addWidget(recordScreenBtn); // 添加录制按钮到布局

//     // 新增：录制状态标签
//     recordingStatusLabel = new QLabel("录制状态: 未开始");
//     recordingStatusLabel->setStyleSheet("color: gray;");

//     localLayout->addWidget(myVideoPreview);
//     localLayout->addWidget(new QLabel("摄像头:"));
//     localLayout->addWidget(comboCam_List);
//     localLayout->addWidget(new QLabel("分辨率:"));
//     localLayout->addWidget(comboCam_VideoRes);
//     localLayout->addWidget(new QLabel("帧率:"));
//     localLayout->addWidget(comboCam_FrameRate);
//     localLayout->addLayout(cameraLayout);
//     localLayout->addWidget(recordingStatusLabel); // 添加状态标签

//     // 右侧面板 - 网络设置和远程视频
//     QGroupBox *remoteGroup = new QGroupBox("远程连接");
//     QVBoxLayout *remoteLayout = new QVBoxLayout(remoteGroup);

//     showImgLabel = new QLabel("远程视频预览");
//     showImgLabel->setScaledContents(true);
//     showImgLabel->setMinimumSize(320, 240);
//     showImgLabel->setFrameStyle(QFrame::Box);
//     showImgLabel->setAlignment(Qt::AlignCenter);

//     ipv4Label = new QLabel("本机IP: ");
//     ipEdit = new QLineEdit();
//     ipEdit->setPlaceholderText("目标IP地址");
//     portEdit = new QLineEdit();
//     portEdit->setPlaceholderText("目标端口");
//     connectBtn = new QPushButton("连接");

//     serverPortEdit = new QLineEdit();
//     serverPortEdit->setPlaceholderText("监听端口");
//     startServerBtn = new QPushButton("启动服务器");

//     remoteLayout->addWidget(showImgLabel);
//     remoteLayout->addWidget(ipv4Label);
//     remoteLayout->addWidget(new QLabel("目标地址:"));
//     remoteLayout->addWidget(ipEdit);
//     remoteLayout->addWidget(new QLabel("目标端口:"));
//     remoteLayout->addWidget(portEdit);
//     remoteLayout->addWidget(connectBtn);
//     remoteLayout->addWidget(new QLabel("监听端口:"));
//     remoteLayout->addWidget(serverPortEdit);
//     remoteLayout->addWidget(startServerBtn);

//     ConferenceUi *message = new ConferenceUi();


//     // 添加到主布局
//     leftLayout->addWidget(localGroup);
//     rightLayout->addWidget(remoteGroup);
//     mainLayout->addLayout(leftLayout);
//     mainLayout->addLayout(rightLayout);

//     rrLayout->addWidget(message);
//     mainLayout->addLayout(rrLayout);

//     // 连接信号槽
//     connect(comboCam_List, SIGNAL(currentIndexChanged(int)), this, SLOT(handle_ShowCameraDevice(int)));
//     connect(startServerBtn, SIGNAL(clicked()), this, SLOT(on_startServerBtn_clicked()));
//     connect(connectBtn, SIGNAL(clicked()), this, SLOT(on_connectBtn_clicked()));
//     connect(toggleCameraBtn, SIGNAL(clicked()), this, SLOT(on_toggleCameraBtn_clicked()));
//     connect(toggleMicBtn, SIGNAL(clicked()), this, SLOT(on_toggleMicBtn_clicked()));
//     connect(recordScreenBtn, SIGNAL(clicked()), this, SLOT(on_recordScreenBtn_clicked())); // 连接录制按钮

//     qDebug() << "UI Rendered";
//     // 初始化
//     init_ConferenceWindowInfo();
//     get_LocalIPv4();

//     // 初始化音频格式
//     setupAudioFormat();
//     // 创建UDP套接字
//     udpAudioSocket = new QUdpSocket(this);
// }

// 尝试自动连接
ConferenceWindow::ConferenceWindow(const QString &ip, quint16 port, QWidget *parent)
    : QMainWindow(parent), camera_Obj(nullptr), myVideoPreview(nullptr),
    capture_Obj(nullptr), audioInput(nullptr),
    tcpSocket(nullptr), tcpServer(nullptr), timer_Obj(nullptr), video_ThreadObj(nullptr),
    udpAudioSocket(nullptr),
    audioOutput(nullptr),
    audioInputDevice(nullptr),
    audioOutputDevice(nullptr),
    audioSenderThread(nullptr),
    audioReceiverThread(nullptr),
    audioEnabled(false),
    remoteAudioReady(false),
    ffmpegProcess(nullptr),
    isRecording(false),
    autoConnectIp(ip),
    autoConnectPort(port),
    autoConnectEnabled(!ip.isEmpty() && port > 0)
{
    // 创建主窗口控件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QVBoxLayout *rrLayout = new QVBoxLayout();

    // 左侧面板 - 本地视频和设置
    QGroupBox *localGroup = new QGroupBox("本地设置");
    QVBoxLayout *localLayout = new QVBoxLayout(localGroup);

    // 使用QVideoWidget作为视频预览
    myVideoPreview = new QVideoWidget();
    myVideoPreview->setMinimumSize(320, 240);

    comboCam_List = new QComboBox();
    comboCam_VideoRes = new QComboBox();
    comboCam_FrameRate = new QComboBox();

    QHBoxLayout *cameraLayout = new QHBoxLayout();
    toggleCameraBtn = new QPushButton("关闭摄像头");
    toggleMicBtn = new QPushButton("开启麦克风");
    recordScreenBtn = new QPushButton("开始录制屏幕"); // 新增：录制屏幕按钮
    cameraLayout->addWidget(toggleCameraBtn);
    cameraLayout->addWidget(toggleMicBtn);
    cameraLayout->addWidget(recordScreenBtn); // 添加录制按钮到布局

    // 新增：录制状态标签
    recordingStatusLabel = new QLabel("录制状态: 未开始");
    recordingStatusLabel->setStyleSheet("color: gray;");

    localLayout->addWidget(myVideoPreview);
    localLayout->addWidget(new QLabel("摄像头:"));
    localLayout->addWidget(comboCam_List);
    localLayout->addWidget(new QLabel("分辨率:"));
    localLayout->addWidget(comboCam_VideoRes);
    localLayout->addWidget(new QLabel("帧率:"));
    localLayout->addWidget(comboCam_FrameRate);
    localLayout->addLayout(cameraLayout);
    localLayout->addWidget(recordingStatusLabel); // 添加状态标签

    // 右侧面板 - 网络设置和远程视频
    QGroupBox *remoteGroup = new QGroupBox("远程连接");
    QVBoxLayout *remoteLayout = new QVBoxLayout(remoteGroup);

    showImgLabel = new QLabel("远程视频预览");
    showImgLabel->setScaledContents(true);
    showImgLabel->setMinimumSize(320, 240);
    showImgLabel->setFrameStyle(QFrame::Box);
    showImgLabel->setAlignment(Qt::AlignCenter);

    ipv4Label = new QLabel("本机IP: ");
    ipEdit = new QLineEdit();
    ipEdit->setPlaceholderText("目标IP地址");
    portEdit = new QLineEdit();
    portEdit->setPlaceholderText("目标端口");
    connectBtn = new QPushButton("连接");

    serverPortEdit = new QLineEdit();
    serverPortEdit->setPlaceholderText("监听端口");
    startServerBtn = new QPushButton("启动服务器");

    remoteLayout->addWidget(showImgLabel);
    remoteLayout->addWidget(ipv4Label);
    remoteLayout->addWidget(new QLabel("目标地址:"));
    remoteLayout->addWidget(ipEdit);
    remoteLayout->addWidget(new QLabel("目标端口:"));
    remoteLayout->addWidget(portEdit);
    remoteLayout->addWidget(connectBtn);
    remoteLayout->addWidget(new QLabel("监听端口:"));
    remoteLayout->addWidget(serverPortEdit);
    remoteLayout->addWidget(startServerBtn);

    // ConferenceUi *message = new ConferenceUi();

    // 添加到主布局
    leftLayout->addWidget(localGroup);
    rightLayout->addWidget(remoteGroup);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // rrLayout->addWidget(message);
    mainLayout->addLayout(rrLayout);

    // 连接信号槽
    connect(comboCam_List, SIGNAL(currentIndexChanged(int)), this, SLOT(handle_ShowCameraDevice(int)));
    connect(startServerBtn, SIGNAL(clicked()), this, SLOT(on_startServerBtn_clicked()));
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(on_connectBtn_clicked()));
    connect(toggleCameraBtn, SIGNAL(clicked()), this, SLOT(on_toggleCameraBtn_clicked()));
    connect(toggleMicBtn, SIGNAL(clicked()), this, SLOT(on_toggleMicBtn_clicked()));
    connect(recordScreenBtn, SIGNAL(clicked()), this, SLOT(on_recordScreenBtn_clicked())); // 连接录制按钮

    qDebug() << "UI Rendered";
    // 初始化
    init_ConferenceWindowInfo();
    get_LocalIPv4();

    // 初始化音频格式
    setupAudioFormat();
    // 创建UDP套接字
    udpAudioSocket = new QUdpSocket(this);

    if (autoConnectEnabled) {
        // 使用单次定时器延迟连接，确保UI完全初始化
        QTimer::singleShot(1000, this, &ConferenceWindow::attemptAutoConnect);
    }
}


ConferenceWindow::~ConferenceWindow()
{
    if(timer_Obj) {
        timer_Obj->stop();
        delete timer_Obj;
    }
    if(camera_Obj) {
        camera_Obj->stop();
        delete camera_Obj;
    }
    if(capture_Obj) {
        delete capture_Obj;
    }
    if(audioInput) {
        audioInput->stop();
        delete audioInput;
    }
    if(video_ThreadObj) {
        video_ThreadObj->handle_CloseThread();
    }
    if(myVideoPreview) {
        delete myVideoPreview;
    }

    // 清理音频资源
    stopAudio();

    if (udpAudioSocket) {
        udpAudioSocket->close();
        delete udpAudioSocket;
    }

    if (audioSenderThread) {
        delete audioSenderThread;
    }

    if (audioReceiverThread) {
        delete audioReceiverThread;
    }

    // 清理屏幕录制资源
    if (isRecording) {
        stopScreenRecording();
    }
    if (ffmpegProcess) {
        ffmpegProcess->terminate();
        delete ffmpegProcess;
    }
}

void ConferenceWindow::setupAudioFormat()
{
    audioFormat.setSampleRate(16000); // 16KHz采样率
    audioFormat.setChannelCount(1);   // 单声道
    audioFormat.setSampleSize(16);     // 16位采样大小
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);
}

void ConferenceWindow::initAudio()
{
    // 检查音频设备
    QAudioDeviceInfo inputDevice = QAudioDeviceInfo::defaultInputDevice();
    if (inputDevice.isNull()) {
        QMessageBox::warning(this, "错误", "没有找到可用的音频输入设备");
        return;
    }

    QAudioDeviceInfo outputDevice = QAudioDeviceInfo::defaultOutputDevice();
    if (outputDevice.isNull()) {
        QMessageBox::warning(this, "错误", "没有找到可用的音频输出设备");
        return;
    }

    // 确保支持选择的格式
    QAudioFormat selectedFormat = audioFormat;
    if (!inputDevice.isFormatSupported(selectedFormat)) {
        selectedFormat = inputDevice.nearestFormat(selectedFormat);
        qWarning() << "Default format not supported, using nearest:" << selectedFormat;
    }

    // 创建音频输入
    if (audioInput) {
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
    }
    audioInput = new QAudioInput(inputDevice, selectedFormat, this);

    // 创建音频输出
    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
        audioOutput = nullptr;
    }
    audioOutput = new QAudioOutput(outputDevice, selectedFormat, this);

    // 获取IO设备
    audioInputDevice = audioInput->start();
    audioOutputDevice = audioOutput->start();

    // 连接音频输入信号
    connect(audioInputDevice, &QIODevice::readyRead,
            this, &ConferenceWindow::on_audioDataReady);

    // 连接UDP接收信号
    connect(udpAudioSocket, &QUdpSocket::readyRead,
            this, &ConferenceWindow::on_audioDataReceived);

    audioEnabled = true;
    qDebug() << "Audio initialized";
}

void ConferenceWindow::stopAudio()
{
    if (audioInput) {
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
    }

    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
        audioOutput = nullptr;
    }

    audioInputDevice = nullptr;
    audioOutputDevice = nullptr;

    if (audioSenderThread) {
        audioSenderThread->stop();
        delete audioSenderThread;
        audioSenderThread = nullptr;
    }

    if (audioReceiverThread) {
        audioReceiverThread->stop();
        delete audioReceiverThread;
        audioReceiverThread = nullptr;
    }

    audioEnabled = false;
    qDebug() << "Audio stopped";
}

void ConferenceWindow::init_ConferenceWindowInfo()
{
    // 获取默认摄像头
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.isEmpty()) {
        QMessageBox::critical(this,"提示","没有发现摄像头");
        return;
    }
    // 检查是否有默认摄像头或选择最合适的摄像头
    QCameraInfo defaultCamera = QCameraInfo::defaultCamera();
    if (!defaultCamera.isNull() && cameras.contains(defaultCamera)) {
        qInfo() << "default Camera";
        init_CameraDevice(defaultCamera);
    } else {
        qInfo() << "camera Init";
        init_CameraDevice(cameras.first());
    }
}

void ConferenceWindow::init_CameraDevice(const QCameraInfo& device)
{
    qInfo() << "Init Camera Started";
    // 初始化摄像头列表
    comboCam_List->blockSignals(true);
    comboCam_List->clear();
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    // 检查是否有可用摄像头

    int selectedIndex = -1;
    for (int i = 0; i < cameras.size(); i++) {
        comboCam_List->addItem(cameras[i].description(), QVariant::fromValue(cameras[i]));
        if (cameras[i].deviceName() == device.deviceName()) {
            selectedIndex = i;
        }
    }

    if (selectedIndex >= 0) {
        comboCam_List->setCurrentIndex(selectedIndex);
    }
    comboCam_List->blockSignals(false);

    // 设置摄像头
    if (camera_Obj) {
        camera_Obj->stop();
        delete camera_Obj;
        camera_Obj = nullptr;
    }

    // 初始化图像捕获
    if (capture_Obj) {
        delete capture_Obj;
        capture_Obj = nullptr;
    }

    // 创建新摄像头实例
    try {
        camera_Obj = new QCamera(device);

        // 检查摄像头是否有效
        if (camera_Obj->error() != QCamera::NoError) {
            qDebug() << "Camera error:" << camera_Obj->errorString();
            delete camera_Obj;
            camera_Obj = nullptr;
            return;
        }

        camera_Obj->setViewfinder(myVideoPreview);

        // 初始化图像捕获
        capture_Obj = new QCameraImageCapture(camera_Obj);
        connect(capture_Obj, SIGNAL(imageCaptured(int, QImage)),
                this, SLOT(on_imageCaptured(int, QImage)));

        // 启动摄像头
        camera_Obj->start();

        // 延迟初始化摄像头信息，确保摄像头已启动
        QTimer::singleShot(1000, this, SLOT(init_CameraDeviceInfo()));

    } catch (QException &e) {
        qDebug() << "Failed to initialize camera device: " << e.what();
        if (camera_Obj) {
            delete camera_Obj;
            camera_Obj = nullptr;
        }
    }
}

void ConferenceWindow::init_CameraDeviceInfo()
{
    comboCam_VideoRes->clear();
    comboCam_FrameRate->clear();

    // 获取支持的分辨率
    if (capture_Obj) {
        QList<QSize> resolutions = capture_Obj->supportedResolutions();
        foreach (const QSize &res, resolutions) {
            QString resStr = QString("%1 x %2").arg(res.width()).arg(res.height());
            comboCam_VideoRes->addItem(resStr);
        }
    }

    // 帧率设置（简化处理）
    comboCam_FrameRate->addItems({"10", "15", "30", "60"});
    comboCam_FrameRate->setCurrentIndex(1); // 默认15fps
}

void ConferenceWindow::handle_ShowCameraDevice(int index)
{
    // 从 QVariant 中获取 QCameraInfo
    QVariant var = comboCam_List->itemData(index);
    if (var.canConvert<QCameraInfo>()) {
        QCameraInfo device = var.value<QCameraInfo>();
        init_CameraDevice(device);
    }
}

void ConferenceWindow::on_startServerBtn_clicked()
{
    qDebug() << "服务器启动，监听端口:" << serverPortEdit->text();
    init_TcpModule();

    // 绑定音频UDP端口
    quint16 audioPort = serverPortEdit->text().toUShort() + 1;
    if (!udpAudioSocket->bind(QHostAddress::Any, audioPort)) {
        qWarning() << "Audio UDP bind failed:" << udpAudioSocket->errorString();
    } else {
        qDebug() << "Audio UDP bound to port:" << audioPort;
    }
}

void ConferenceWindow::on_connectBtn_clicked()
{
    qDebug() << "尝试连接到:" << ipEdit->text() << ":" << portEdit->text();

    QString ip = ipEdit->text();
    QString port = portEdit->text();

    if(port.isEmpty() || ip.isEmpty()){
        QMessageBox::information(this, "提示", "未输入目标端口或IPv4地址", QMessageBox::Ok);
        return;
    }

    if(tcpSocket) {
        tcpSocket->deleteLater();
        tcpSocket = nullptr;
    }

    if(video_ThreadObj) {
        video_ThreadObj->handle_CloseThread();
        delete video_ThreadObj;
        video_ThreadObj = nullptr;
    }

    tcpSocket = new QTcpSocket(this);
    qDebug() << "发起连接，目标地址：" << ip << "目标端口" << port.toUShort();

    connect(tcpSocket, &QAbstractSocket::connected, this, &ConferenceWindow::handle_Connect);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ConferenceWindow::hanlde_RelayinThread);

    tcpSocket->connectToHost(ip, port.toUShort());

    // 创建并启动视频接收线程
    video_ThreadObj = new VideoThread(showImgLabel);
    QThreadPool::globalInstance()->start(video_ThreadObj);

    // 如果是主动连接方，创建发送线程
    QHostAddress targetAddress(ip);
    if (targetAddress.isNull()) {
        qWarning() << "Invalid target address for audio";
    } else {
        // 音频端口为视频端口+1
        quint16 audioPort = port.toUShort() + 1;
        remoteAudioAddress = targetAddress;
        remoteAudioPort = audioPort;

        // 创建发送线程
        audioSenderThread = new AudioThread(AudioThread::Sender,
                                            udpAudioSocket,
                                            nullptr,
                                            targetAddress,
                                            audioPort);
        QThreadPool::globalInstance()->start(audioSenderThread);
    }
}

void ConferenceWindow::handle_Connect()
{
    qDebug() << "连接成功，启动定时器";
    init_TimerModel();
}

void ConferenceWindow::init_TimerModel()
{
    if(timer_Obj) {
        timer_Obj->stop();
        delete timer_Obj;
    }

    timer_Obj = new QTimer(this);
    int fps = comboCam_FrameRate->currentText().toInt();
    int interval = fps > 0 ? 1000 / fps : 100; // 根据选择的帧率设置定时器
    timer_Obj->start(interval);
    connect(timer_Obj, &QTimer::timeout, this, &ConferenceWindow::handle_InterceptImg);
}

void ConferenceWindow::handle_InterceptImg()
{
    if(camera_Obj && camera_Obj->status() == QCamera::ActiveStatus && capture_Obj) {
        capture_Obj->capture();
    }
}

void ConferenceWindow::on_imageCaptured(int id, const QImage &image)
{
    Q_UNUSED(id);

    // 发送图片
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);

        // 提高压缩质量，减少频闪
        QImage scaledImage = image.scaledToWidth(640, Qt::SmoothTransformation);

        // 确保保存为JPEG格式
        if (!scaledImage.save(&buffer, "JPEG", 70)) {
            qDebug() << "无法将图像保存为JPEG格式";
            return;
        }

        // 添加调试输出
        qDebug() << "准备发送图片大小:" << imageData.size() << "字节";

        // 先发送图像大小信息
        quint32 imageSize = imageData.size();
        QByteArray sizeData(reinterpret_cast<const char*>(&imageSize), sizeof(quint32));
        tcpSocket->write(sizeData);

        // 再发送图像数据
        tcpSocket->write(imageData);

        if (!tcpSocket->flush()) {
            qDebug() << "发送数据失败";
        }
    }
}

void ConferenceWindow::init_TcpModule()
{
    if (tcpServer) {
        tcpServer->close();
        delete tcpServer;
        tcpServer = nullptr;
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &ConferenceWindow::init_TcpConnection);

    QString port = serverPortEdit->text();
    if(port.isEmpty()){
        QMessageBox::information(this, "提示", "请输入监听端口", QMessageBox::Ok);
        return;
    }

    qDebug() << "监听端口" << port.toUShort();
    if (!tcpServer->listen(QHostAddress::Any, port.toUShort())) {
        QMessageBox::critical(this, "错误", "无法启动服务器: " + tcpServer->errorString());
    }
}

void ConferenceWindow::init_TcpConnection()
{
    qDebug() << "获取到连接";
    if (tcpSocket) {
        tcpSocket->deleteLater();
        tcpSocket = nullptr;
    }
    tcpSocket = tcpServer->nextPendingConnection();

    if (video_ThreadObj) {
        video_ThreadObj->handle_CloseThread();
        delete video_ThreadObj;
        video_ThreadObj = nullptr;
    }

    // 创建并启动视频接收线程
    video_ThreadObj = new VideoThread(showImgLabel);
    QThreadPool::globalInstance()->start(video_ThreadObj);

    connect(tcpSocket, &QAbstractSocket::readyRead, this, &ConferenceWindow::hanlde_RelayinThread);
    connect(tcpSocket, &QAbstractSocket::connected, this, &ConferenceWindow::handle_Connect);

    // 服务器端也需要启动定时器发送视频
    handle_Connect();

    // 创建音频接收线程
    audioReceiverThread = new AudioThread(AudioThread::Receiver,
                                          udpAudioSocket,
                                          audioOutputDevice,
                                          QHostAddress(),
                                          0);
    QThreadPool::globalInstance()->start(audioReceiverThread);
}

void ConferenceWindow::hanlde_RelayinThread()
{
    if(!video_ThreadObj || !tcpSocket) {
        qDebug() << "视频线程或套接字未初始化";
        return;
    }

    if(tcpSocket->bytesAvailable() > 0){
        // 读取图像大小信息
        if (tcpSocket->bytesAvailable() >= sizeof(quint32)) {
            QByteArray sizeData = tcpSocket->read(sizeof(quint32));
            quint32 imageSize = *reinterpret_cast<const quint32*>(sizeData.constData());
            qDebug() << "接收到图像大小信息:" << imageSize << "字节";

            // 等待完整图像数据到达
            int waitTime = 0;
            while (tcpSocket->bytesAvailable() < imageSize) {
                if (!tcpSocket->waitForReadyRead(100)) {
                    waitTime += 100;
                    if (waitTime > 5000) {
                        qDebug() << "等待图像数据超时";
                        return;
                    }
                }
            }

            // 读取完整图像数据
            QByteArray data = tcpSocket->read(imageSize);
            qDebug() << "接收到完整图像数据:" << data.size() << "字节";

            // 使用线程安全的方式调用视频显示
            QMetaObject::invokeMethod(video_ThreadObj, "handle_ShowReceiveImg",
                                      Qt::QueuedConnection,
                                      Q_ARG(QByteArray, data));
        } else {
            qDebug() << "数据不足，无法读取图像大小信息";
        }
    }
}


int ConferenceWindow::get_LocalIPv4()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).protocol() == QAbstractSocket::IPv4Protocol) {
            ipv4Label->setText("本机IP: " + ipAddressesList.at(i).toString());
            return 1;
        }
    }

    // 没有找到IPv4地址，使用localhost
    ipv4Label->setText("本机IP: 127.0.0.1");
    return 0;
}


void ConferenceWindow::on_toggleCameraBtn_clicked()
{
    if (camera_Obj) {
        if (camera_Obj->status() == QCamera::ActiveStatus) {
            camera_Obj->stop();
            toggleCameraBtn->setText("开启摄像头");
        } else {
            camera_Obj->start();
            toggleCameraBtn->setText("关闭摄像头");
        }
    }
}


void ConferenceWindow::on_toggleMicBtn_clicked()
{
    if (toggleMicBtn->text() == "开启麦克风") {
        // 初始化音频设备
        initAudio();
        toggleMicBtn->setText("关闭麦克风");
        qDebug() << "麦克风已开启";
    }
    else if (toggleMicBtn->text() == "关闭麦克风") {
        // 停止音频
        stopAudio();
        toggleMicBtn->setText("开启麦克风");
        qDebug() << "麦克风已关闭";
    }
}

void ConferenceWindow::on_audioDataReady()
{
    if (audioSenderThread && audioInputDevice) {
        QByteArray data = audioInputDevice->readAll();
        if (!data.isEmpty()) {
            QMetaObject::invokeMethod(audioSenderThread, "handleData",
                                      Qt::QueuedConnection,
                                      Q_ARG(QByteArray, data));
        }
    }
}

void ConferenceWindow::on_audioDataReceived()
{
    // 数据由接收线程处理，这里不需要做额外处理
}

void ConferenceWindow::on_recordScreenBtn_clicked()
{
    if (!isRecording) {
        // 开始录制
        startScreenRecording();
        recordScreenBtn->setText("停止录制屏幕");
        recordingStatusLabel->setText("录制状态: 录制中...");
        recordingStatusLabel->setStyleSheet("color: red;");
    } else {
        // 停止录制
        stopScreenRecording();
        recordScreenBtn->setText("开始录制屏幕");
        recordingStatusLabel->setText("录制状态: 已停止");
        recordingStatusLabel->setStyleSheet("color: gray;");
    }
}

void ConferenceWindow::startScreenRecording()
{
    // 确保窗口可见且激活
    this->window()->showNormal();
    this->window()->raise();
    this->window()->activateWindow();
    QApplication::processEvents();

    // 获取整个主窗口的几何信息（包括标题栏和边框）
    QWidget* mainWindow = this->window();
    QRect windowRect = mainWindow->frameGeometry();

    // 确保宽度和高度都是2的倍数，这是大多数视频编码器的要求
    int adjustedWidth = windowRect.width();
    if (adjustedWidth % 2 != 0) {
        adjustedWidth--;
    }
    int adjustedHeight = windowRect.height();
    if (adjustedHeight % 2 != 0) {
        adjustedHeight--;
    }

    if (adjustedWidth <= 0 || adjustedHeight <= 0) {
        QMessageBox::warning(this, "录制错误", "计算出的窗口尺寸无效，无法开始录制。");
        return;
    }

    QSize adjustedSize(adjustedWidth, adjustedHeight);

    qDebug() << "主窗口完整尺寸 (frameGeometry): " << windowRect.size();
    qDebug() << "调整后录制尺寸: " << adjustedSize;

    // x11grab 的偏移量是相对于X服务器根窗口的
    // 所以这里的 x, y 应该是窗口左上角相对于屏幕左上角的坐标
    QString windowSize = QString("%1x%2").arg(adjustedSize.width()).arg(adjustedSize.height());
    QString windowPosition = QString("%1,%2").arg(windowRect.x()).arg(windowRect.y());

    // 生成文件名
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if (documentsPath.isEmpty()) {
        documentsPath = QDir::currentPath();
    }
    videoFileName = documentsPath + "/conference_recording_" + timestamp + ".mp4";

    // 设置FFmpeg命令
    QString program = "ffmpeg";
    QStringList arguments;

    arguments << "-f" << "x11grab"
              << "-video_size" << windowSize // 录制我们调整后的窗口大小
              << "-draw_mouse" << "1"       // 确保绘制鼠标光标
              << "-i" << QString(":0.0+%1").arg(windowPosition) // 录制我们窗口的位置
              << "-r" << "30.0"             // 帧率 30 fps
              << "-profile:v" << "high444"  // 使用 high444 配置文件
              << "-level" << "5.1"          // 配置文件级别 5.1
              << "-pix_fmt" << "yuv420p"    // 像素格式 YUV420p
              << "-preset:v" << "ultrafast" // 编码预设 ultrafast
              << "-y"                       // 覆盖已有文件
              << videoFileName;

    // 清空上次的错误信息
    lastFfmpegError.clear();

    // 创建FFmpeg进程
    if (ffmpegProcess) {
        ffmpegProcess->terminate();
        delete ffmpegProcess;
        ffmpegProcess = nullptr; // 重置指针
    }
    ffmpegProcess = new QProcess(this);

    // 确保日志目录存在
    QString logDirPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir logDir(logDirPath);
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    // 创建日志文件路径
    logFileName = logDir.filePath("ffmpeg_log_" + timestamp + ".txt");

    // 连接信号
    connect(ffmpegProcess, &QProcess::readyReadStandardError, [=]() {
        QByteArray errorData = ffmpegProcess->readAllStandardError();
        qDebug() << "FFmpeg error:" << errorData;

        // 将错误信息保存为成员变量
        lastFfmpegError += QString::fromUtf8(errorData);

        // 尝试写入日志文件
        QFile logFile(logFileName);
        if (logFile.open(QIODevice::Append | QIODevice::Text)) {
            logFile.write(errorData);
            logFile.close();
        }
    });

    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                qDebug() << "FFmpeg finished with code:" << exitCode;
                if (isRecording) { // 只有在正在录制状态下才处理完成事件
                    isRecording = false; // 停止录制状态
                    recordScreenBtn->setText("开始录制屏幕");
                    recordingStatusLabel->setText("录制状态: 已停止");
                    recordingStatusLabel->setStyleSheet("color: gray;");

                    if (exitCode == 0) {
                        QMessageBox::information(this, "录制完成",
                                                 QString("会议录制已保存到:\n%1").arg(videoFileName));
                    } else {
                        QString errorDetails = QString("录制失败，退出代码: %1\n\n").arg(exitCode);

                        if (!lastFfmpegError.isEmpty()) {
                            errorDetails += "FFmpeg错误输出:\n" + lastFfmpegError;
                        } else {
                            errorDetails += "FFmpeg进程异常终止，但没有捕获到标准错误输出。\n请检查日志文件: " + logFileName;
                        }

                        QMessageBox::warning(this, "录制失败", errorDetails);
                    }
                }
            });

    // 启动FFmpeg
    qDebug() << "Executing FFmpeg command: " << program << arguments.join(" ");
    ffmpegProcess->start(program, arguments);

    if (ffmpegProcess->waitForStarted(3000)) {
        isRecording = true;
        qDebug() << "开始会议录制，保存到:" << videoFileName;
        qDebug() << "录制区域尺寸:" << windowSize << "位置:" << windowPosition;

        recordScreenBtn->setText("停止录制屏幕");
        recordingStatusLabel->setText("录制状态: 录制中...");
        recordingStatusLabel->setStyleSheet("color: red;");
    } else {
        QString errorDetails;
        switch (ffmpegProcess->error()) {
        case QProcess::FailedToStart:
            errorDetails = "FFmpeg进程无法启动。请检查：\n"
                           "1. FFmpeg是否已安装且在系统PATH中可执行？ (尝试在终端运行 'ffmpeg -version')\n"
                           "2. 是否安装了 X11 grabber 相关的依赖？ (如 sudo apt install libx11-dev libxtst-dev libxcb-randr0-dev)";
            break;
        case QProcess::Crashed:
            errorDetails = "FFmpeg进程崩溃";
            break;
        case QProcess::Timedout:
            errorDetails = "启动FFmpeg超时";
            break;
        default:
            errorDetails = "未知错误：" + ffmpegProcess->errorString();
        }

        QString fullError = QString("无法启动FFmpeg: %1\n"
                                    "详细错误: %2")
                                .arg(ffmpegProcess->errorString())
                                .arg(errorDetails);

        QMessageBox::critical(this, "FFmpeg错误", fullError);
        isRecording = false;
        recordScreenBtn->setText("开始录制屏幕");
        recordingStatusLabel->setText("录制状态: 已停止");
        recordingStatusLabel->setStyleSheet("color: gray;");
    }
}


void ConferenceWindow::stopScreenRecording()
{
    if (isRecording && ffmpegProcess) {
        // 发送结束信号
        ffmpegProcess->write("q\n");
        ffmpegProcess->waitForBytesWritten();

        // 给予最长30秒完成时间
        int maxWaitTime = 30; // 秒
        int waited = 0;

        while (ffmpegProcess->state() == QProcess::Running && waited < maxWaitTime) {
            if (ffmpegProcess->waitForFinished(1000)) break;
            waited++;
            recordingStatusLabel->setText(QString("录制状态: 正在结束(%1/%2秒)").arg(waited).arg(maxWaitTime));
            QApplication::processEvents(); // 更新UI
        }

        if (ffmpegProcess->state() == QProcess::Running) {
            // 强制终止
            ffmpegProcess->terminate();
            ffmpegProcess->waitForFinished(1000);

            // 检查文件是否存在
            QFileInfo fileInfo(videoFileName);
            QString message = fileInfo.exists() ?
                                  QString("录制部分完成，文件大小: %1 MB").arg(fileInfo.size() / (1024.0 * 1024.0), 0, 'f', 2) :
                                  "录制失败，未生成文件";

            QMessageBox::warning(this, "录制终止", message);
        } else if (ffmpegProcess->exitStatus() == QProcess::NormalExit) {
            QMessageBox::information(this, "录制完成",
                                     "屏幕录制已保存到:\n" + videoFileName);
        } else {
            QMessageBox::warning(this, "录制错误",
                                 "录制异常结束: " + ffmpegProcess->errorString());
        }

        isRecording = false;
        recordScreenBtn->setText("开始录制屏幕");
        recordingStatusLabel->setText("录制状态: 已停止");
        recordingStatusLabel->setStyleSheet("color: gray;");
    }
}

// 添加自动连接实现
void ConferenceWindow::attemptAutoConnect()
{
    if (!autoConnectEnabled) {
        return;
    }

    // 设置UI上的IP和端口
    ipEdit->setText(autoConnectIp);
    portEdit->setText(QString::number(autoConnectPort));

    qDebug() << "尝试自动连接到:" << autoConnectIp << ":" << autoConnectPort;

    // 模拟点击连接按钮
    on_connectBtn_clicked();
}
