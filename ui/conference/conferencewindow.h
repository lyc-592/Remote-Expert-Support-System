#ifndef CONFERENCEWINDOW_H
#define CONFERENCEWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QAudioInput>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QVideoWidget>
#include <QUdpSocket>   // 新增：用于音频传输
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput> // 新增：用于音频输出
#include <QProcess>    // 新增：用于FFmpeg进程
#include <QDesktopWidget> // 新增：获取屏幕尺寸

Q_DECLARE_METATYPE(QCameraInfo)

class VideoThread;
class AudioThread;  // 新增：音频线程类声明

class ConferenceWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConferenceWindow(const QString &ip, quint16 port, QWidget *parent = nullptr);
    ~ConferenceWindow();

private slots:
    void init_ConferenceWindowInfo();
    void init_CameraDevice(const QCameraInfo& device);
    void init_CameraDeviceInfo();
    void handle_ShowCameraDevice(int index);
    void on_startServerBtn_clicked();
    void on_connectBtn_clicked();
    void handle_Connect();
    void init_TimerModel();
    void handle_InterceptImg();
    void on_imageCaptured(int id, const QImage &image);
    void init_TcpModule();
    void init_TcpConnection();
    void hanlde_RelayinThread();
    int get_LocalIPv4();
    void on_toggleCameraBtn_clicked();
    void on_toggleMicBtn_clicked();
    void on_recordScreenBtn_clicked(); // 新增：屏幕录制按钮点击事件

    // 新增音频槽函数
    void on_audioDataReady();
    void on_audioDataReceived();

private:
    QCamera *camera_Obj;
    QVideoWidget *myVideoPreview;
    QCameraImageCapture *capture_Obj;
    QAudioInput *audioInput;
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    QTimer *timer_Obj;
    VideoThread *video_ThreadObj;

    // UI 控件
    QComboBox *comboCam_List;
    QComboBox *comboCam_VideoRes;
    QComboBox *comboCam_FrameRate;
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QLineEdit *serverPortEdit;
    QPushButton *startServerBtn;
    QPushButton *connectBtn;
    QPushButton *toggleCameraBtn;
    QPushButton *toggleMicBtn;
    QPushButton *recordScreenBtn; // 新增：屏幕录制按钮
    QLabel *showImgLabel;
    QLabel *ipv4Label;
    QLabel *recordingStatusLabel; // 新增：录制状态标签

    // 新增音频相关成员
    QUdpSocket *udpAudioSocket;
    QAudioFormat audioFormat;
    QAudioOutput *audioOutput;
    QIODevice *audioInputDevice;
    QIODevice *audioOutputDevice;
    AudioThread *audioSenderThread;
    AudioThread *audioReceiverThread;
    bool audioEnabled;
    bool remoteAudioReady;
    QHostAddress remoteAudioAddress;
    quint16 remoteAudioPort;

    // 新增屏幕录制相关成员
    QProcess *ffmpegProcess; // FFmpeg进程
    bool isRecording;        // 录制状态标志
    QString videoFileName;   // 录制的视频文件名

    QString logFileName;      // 日志文件路径
    QString lastFfmpegError;   // 最后捕获的FFmpeg错误信息

    QString autoConnectIp;
    quint16 autoConnectPort;
    bool autoConnectEnabled;
    void attemptAutoConnect();

    // 初始化音频
    void setupAudioFormat();
    void initAudio();
    void stopAudio();

    // 屏幕录制方法
    void startScreenRecording();
    void stopScreenRecording();
};

#endif // CONFERENCEWINDOW_H
