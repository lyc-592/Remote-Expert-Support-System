// mainconferencewindow.cpp
#include "mainconferencewindow.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

MainConferenceWindow::MainConferenceWindow(QWidget *parent)
    : QMainWindow(parent)
{
    conferenceWindow = new ConferenceWindow("", 0, this);
    conferenceUi = new ConferenceUi(this);

    setupUI();
    // connectSignals();
}

MainConferenceWindow::~MainConferenceWindow()
{
    // 子窗口会自动删除
}

void MainConferenceWindow::setupUI()
{
    // 设置窗口标题
    setWindowTitle("视频会议系统");

    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);

    // 创建分割器
    mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->setHandleWidth(2);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #cccccc; }");

    // 添加窗口到分割器
    mainSplitter->addWidget(conferenceWindow);
    mainSplitter->addWidget(conferenceUi);

    // 设置分割比例 (视频会议占70%，聊天占30%)
    mainSplitter->setStretchFactor(0, 6);
    mainSplitter->setStretchFactor(1, 4);

    mainLayout->addWidget(mainSplitter);

    // 设置初始窗口大小 (屏幕宽度的80%，高度的80%)
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int width = screenGeometry.width() * 0.6;
    int height = screenGeometry.height() * 0.6;
    resize(width, height);

    // 居中显示
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            QApplication::desktop()->availableGeometry()
            )
        );

}

// void MainConferenceWindow::connectSignals()
// {
//     // 如果需要连接两个窗口之间的信号和槽，可以在这里添加
//     // 例如，当会议状态变化时更新聊天界面等
// }
