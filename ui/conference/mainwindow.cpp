#include "mainwindow.h"
#include "videothread.h"
#include <QAudioDeviceInfo>
#include <QCameraInfo>
#include <QDir>
#include <QHostInfo>
#include <QDebug>
#include <QBuffer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QThreadPool>      // 添加头文件
#include <QNetworkInterface> // 添加头文件
#include <QException>

static const int qCameraInfoMetaTypeId = qRegisterMetaType<QCameraInfo>();

ConferenceWindow::ConferenceWindow(QWidget *parent)
    : QMainWindow(parent), camera_Obj(nullptr), myVideoPreview(nullptr),
    capture_Obj(nullptr), audioInput(nullptr),
    tcpSocket(nullptr), tcpServer(nullptr), timer_Obj(nullptr), video_ThreadObj(nullptr),
    audioInitialized(false) // 初始化音频状态标志
{
    // 创建主窗口控件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

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
    toggleMicBtn = new QPushButton("开启麦克风"); // 修改初始文本为"开启麦克风"
    cameraLayout->addWidget(toggleCameraBtn);
    cameraLayout->addWidget(toggleMicBtn);

    localLayout->addWidget(myVideoPreview);
    localLayout->addWidget(new QLabel("摄像头:"));
    localLayout->addWidget(comboCam_List);
    localLayout->addWidget(new QLabel("分辨率:"));
    localLayout->addWidget(comboCam_VideoRes);
    localLayout->addWidget(new QLabel("帧率:"));
    localLayout->addWidget(comboCam_FrameRate);
    localLayout->addLayout(cameraLayout);

    // 右侧面板 - 网络设置和远程视频
    QGroupBox *remoteGroup = new QGroupBox("远程连接");
    QVBoxLayout *remoteLayout = new QVBoxLayout(remoteGroup);

    showImgLabel = new QLabel("远程视频预览");
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

    // 添加到主布局
    leftLayout->addWidget(localGroup);
    rightLayout->addWidget(remoteGroup);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // 连接信号槽
    connect(comboCam_List, SIGNAL(currentIndexChanged(int)), this, SLOT(handle_ShowCameraDevice(int)));
    connect(startServerBtn, SIGNAL(clicked()), this, SLOT(on_startServerBtn_clicked()));
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(on_connectBtn_clicked()));
    connect(toggleCameraBtn, SIGNAL(clicked()), this, SLOT(on_toggleCameraBtn_clicked()));
    connect(toggleMicBtn, SIGNAL(clicked()), this, SLOT(on_toggleMicBtn_clicked()));

    qDebug() << "UI Rendered";
    // 初始化
    init_MainWindowInfo();
    get_LocalIPv4();
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
        audioInput->stop(); // 确保停止音频输入
        delete audioInput;
    }
    if(video_ThreadObj) {
        video_ThreadObj->handle_CloseThread();
    }
    if(myVideoPreview) {
        delete myVideoPreview;
    }
}

void ConferenceWindow::init_MainWindowInfo()
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

        // 不再在摄像头初始化时自动初始化音频设备

        // 启动摄像头
        camera_Obj->start();

        // 延迟初始化摄像头信息，确保摄像头已启动
        QTimer::singleShot(1000, this, SLOT(init_CameraDeviceInfo()));

    } catch (QException &e) { // 修复异常捕获
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
    init_TcpModule();
}

void ConferenceWindow::on_connectBtn_clicked()
{
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
        video_ThreadObj = nullptr;
    }

    tcpSocket = new QTcpSocket(this);
    qDebug() << "发起连接，目标地址：" << ip << "目标端口" << port.toUShort();

    connect(tcpSocket, &QAbstractSocket::connected, this, &ConferenceWindow::handle_Connect);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ConferenceWindow::hanlde_RelayinThread);

    tcpSocket->connectToHost(ip, port.toUShort());
    video_ThreadObj = new VideoThread(showImgLabel);
}

void ConferenceWindow::handle_Connect()
{
    qDebug() << "连接成功";
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
        scaledImage.save(&buffer, "JPEG", 70);

        tcpSocket->write(imageData);
        tcpSocket->flush();
        qDebug() << "发送图片大小:" << imageData.size() << "字节";
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
    video_ThreadObj = new VideoThread(showImgLabel);

    connect(tcpSocket, &QAbstractSocket::readyRead, this, &ConferenceWindow::hanlde_RelayinThread);
    connect(tcpSocket, &QAbstractSocket::connected, this, &ConferenceWindow::handle_Connect);

    QThreadPool::globalInstance()->start(video_ThreadObj);
}

void ConferenceWindow::hanlde_RelayinThread()
{
    if(video_ThreadObj && tcpSocket && tcpSocket->bytesAvailable() > 0){
        QByteArray data = tcpSocket->readAll();
        video_ThreadObj->handle_ShowReceiveImg(data);
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
    // 麦克风未开启的情况 - 开启麦克风
    if (toggleMicBtn->text() == "开启麦克风") {
        // 初始化麦克风
        QAudioDeviceInfo audioDevice = QAudioDeviceInfo::defaultInputDevice();
        if (audioDevice.isNull()) {
            QMessageBox::warning(this, "错误", "没有找到可用的音频输入设备");
            return;
        }

        QAudioFormat format;
        format.setSampleRate(44100);
        format.setChannelCount(1);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::SignedInt);

        if (!audioDevice.isFormatSupported(format)) {
            format = audioDevice.nearestFormat(format);
        }

        // 如果已有音频输入对象，先清理
        if (audioInput) {
            audioInput->stop();
            delete audioInput;
            audioInput = nullptr;
        }

        audioInput = new QAudioInput(format, this);
        audioInput->start();
        toggleMicBtn->setText("关闭麦克风");
        qDebug() << "麦克风已开启";
    }
    // 麦克风已开启的情况 - 关闭麦克风
    else if (toggleMicBtn->text() == "关闭麦克风") {
        if (audioInput) {
            audioInput->stop();
            delete audioInput;
            audioInput = nullptr;
        }
        toggleMicBtn->setText("开启麦克风");
        qDebug() << "麦克风已关闭";
    }
}
