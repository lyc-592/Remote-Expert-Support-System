#include "expertmainwindow.h"
#include "../factory/agendapage.h"
#include "../factory/devicepage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QDebug>

// ============================
// ExpertMainWindow 构造函数
// ============================
ExpertMainWindow::ExpertMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("专家端");
    setMinimumSize(500, 400);
    resize(1200, 800);

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // 右边 QStackedWidget (要最先初始化)
    stackedWidget = new QStackedWidget;

    // ================= leftSidebar =================
    QWidget *sidebar = new QWidget;
    sidebar->setObjectName("Sidebar");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);

    // ExpertMainWindow.cpp 中修改图标加载部分
    QStringList iconPaths = {
        ":/qss/icons/home.svg",       // 首页图标
        ":/qss/icons/user.svg",       // 用户管理图标
        ":/qss/icons/agenda.svg",     // 议程图标
        ":/qss/icons/device.svg",     // 设备图标
        ":/qss/icons/knowledge.svg",  // 企业知识库图标
        ":/qss/icons/history.svg"     // 历史图标
    };
    QStringList tooltips = {"首页", "用户管理", "议程", "设备", "企业知识库", "历史"};

    for (int i = 0; i < iconPaths.size(); ++i) {
        QToolButton *btn = new QToolButton;
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        btn->setIconSize(QSize(32, 32));  // 图标大小
        btn->setMinimumSize(60, 60);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 设置图标（SVG支持）
        btn->setIcon(QIcon(iconPaths[i]));

        // 关键：默认隐藏文字，用tooltip显示名称
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);  // 只显示图标
        btn->setToolTip(tooltips[i]);  // 鼠标悬停时显示文字提示

        sideLayout->addWidget(btn, 1);
        buttons.append(btn);

        connect(btn, &QToolButton::clicked, this, [this, i]() {
            onMenuChanged(i);
        });
    }
    sideLayout->addStretch();

    // 优化悬停提示文字（ToolTip）的样式
    QString styleSheet = R"(
        /* 侧边栏整体样式，这里假设你原来设置了紫色背景等，可根据实际调整 */
        #Sidebar {
            background-color: #7B6DC2; /* 保持原来的紫色背景，你可根据实际修改 */
            border-radius: 10px;
        }

        QToolButton {
            background-color: transparent;
            border: none;
            border-radius: 15px;
            margin: 5px;
            font-family: "Microsoft YaHei";
            font-size: 20px;
            color: white;
        }

        QToolButton:checked {
            background-color: #5a3ea6;
            border-radius: 15px;
        }

        QToolButton:hover {
            background-color: #7d5fc0;
        }

        /* 优化 ToolTip 样式 */
        QToolTip {
            background-color: rgba(50, 50, 50, 0.9); /* 半透明深色背景 */
            color: white; /* 文字白色 */
            padding: 8px 12px; /* 内边距，让提示框更宽敞 */
            border-radius: 6px; /* 圆角 */
            font-size: 14px; /* 文字大小 */
            font-family: "Microsoft YaHei"; /* 字体 */
        }
    )";
    sidebar->setStyleSheet(styleSheet);

    // ================= 页面 =================

    // 0: 首页
    QWidget *homePage = new QWidget;
    {
        QVBoxLayout *homeLayout = new QVBoxLayout(homePage);

        QFrame *overviewCard = new QFrame;
        overviewCard->setObjectName("Card");
        QVBoxLayout *cardLayout = new QVBoxLayout(overviewCard);
        cardLayout->addWidget(new QLabel("📈 专家端首页统计"));
        homeLayout->addWidget(overviewCard);

        QHBoxLayout *bottomCards = new QHBoxLayout;
        QStringList titles = {"待分析工单", "正在处理", "已完成"};
        for (int i = 0; i < 3; i++) {
            QFrame *c = new QFrame;
            c->setObjectName("Card");
            QVBoxLayout *cLayout = new QVBoxLayout(c);
            cLayout->addWidget(new QLabel(titles[i]));
            bottomCards->addWidget(c);
        }
        homeLayout->addLayout(bottomCards);
    }
    stackedWidget->addWidget(homePage);

    // 1: 用户管理
    QWidget *userPage = new QWidget;
    userPage->setLayout(new QVBoxLayout);
    userPage->layout()->addWidget(new QLabel("👤 专家端用户管理页面"));
    stackedWidget->addWidget(userPage);

    // 2: 议程
    stackedWidget->addWidget(new AgendaPage);

    // 3: 设备
    stackedWidget->addWidget(new DevicePage);

    // 4: 企业知识库
    QWidget *knowledgePage = new QWidget;
    knowledgePage->setLayout(new QVBoxLayout);
    knowledgePage->layout()->addWidget(new QLabel("📚 企业知识库页面"));
    stackedWidget->addWidget(knowledgePage);

    // 5: 历史
    QWidget *historyPage = new QWidget;
    historyPage->setLayout(new QVBoxLayout);
    historyPage->layout()->addWidget(new QLabel("📜 历史工单页面"));
    stackedWidget->addWidget(historyPage);

    // 默认显示首页
    stackedWidget->setCurrentIndex(0);
    if (!buttons.isEmpty())
        buttons[0]->setChecked(true);

    // 拼接主布局
    mainLayout->addWidget(sidebar, 1);
    mainLayout->addWidget(stackedWidget, 4);

    setCentralWidget(central);
}

// ============================
// 槽函数：切换菜单
// ============================
void ExpertMainWindow::onMenuChanged(int index)
{
    if (index < 0 || index >= stackedWidget->count()) {
        qWarning() << "Invalid index:" << index;
        return;
    }

    stackedWidget->setCurrentIndex(index);

    for (int j = 0; j < buttons.size(); j++)
        buttons[j]->setChecked(j == index);
}

// ============================
// 析构函数
// ============================
ExpertMainWindow::~ExpertMainWindow() {}
