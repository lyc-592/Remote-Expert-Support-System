#ifndef CONFERENCE_WINDOW_H
#define CONFERENCE_WINDOW_H

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
#include <QVideoWidget>  // 添加视频部件支持

Q_DECLARE_METATYPE(QCameraInfo)

class VideoThread;

class ConferenceWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConferenceWindow(QWidget *parent = nullptr);
    ~ConferenceWindow();

private slots:
    void init_MainWindowInfo();
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

private:
    QCamera *camera_Obj;
    QVideoWidget *myVideoPreview;  // 使用QVideoWidget替代
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
    QLabel *showImgLabel;
    QLabel *ipv4Label;

    // 添加音频状态标志
    bool audioInitialized;
};

#endif // CONFERENCE_WINDOW_H
