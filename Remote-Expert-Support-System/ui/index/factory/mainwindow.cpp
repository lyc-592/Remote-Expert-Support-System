#include "mainwindow.h"
#include "agendapage.h"
#include "devicepage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QDebug>

// ============================
// MainWindow 构造函数
// ============================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    setWindowTitle("工厂端");
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

    QStringList icons = {"首页","用户管理", "议程", "设备", "企业知识库", "历史"};
    for (int i = 0; i < icons.size(); ++i) {
        QToolButton *btn = new QToolButton;
        btn->setCheckable(true);
        btn->setAutoExclusive(true);  // 单选效果
        btn->setIconSize(QSize(32, 32));
        btn->setMinimumSize(60, 60);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setText(icons[i]);

        sideLayout->addWidget(btn, 1);
        buttons.append(btn);

        // 绑定按钮点击 → 切换页面
        connect(btn, &QToolButton::clicked, this, [this, i]() {
            onMenuChanged(i);
        });
    }
    sideLayout->addStretch();

    // ============ 依次添加页面 ============

    // 0: Home Page
    QWidget *homePage = new QWidget;
    {
        QVBoxLayout *homeLayout = new QVBoxLayout(homePage);

        QFrame *overviewCard = new QFrame;
        overviewCard->setObjectName("Card");
        QVBoxLayout *cardLayout = new QVBoxLayout(overviewCard);
        cardLayout->addWidget(new QLabel("📈 Overview Chart Here"));
        homeLayout->addWidget(overviewCard);

        QHBoxLayout *bottomCards = new QHBoxLayout;
        QStringList titles = {"Bicycle Drill", "Jogging Hero", "Healthy Busy"};
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

    // 1: 用户管理 (先用空白页代替)
    QWidget *userPage = new QWidget;
    userPage->setLayout(new QVBoxLayout);
    userPage->layout()->addWidget(new QLabel(" 用户管理页面"));
    stackedWidget->addWidget(userPage);

    // 2: 议程
    stackedWidget->addWidget(new AgendaPage);

    // 3: 设备
    stackedWidget->addWidget(new DevicePage);

    // 4: 联系人
    QWidget *contactsPage = new QWidget;
    contactsPage->setLayout(new QVBoxLayout);
    contactsPage->layout()->addWidget(new QLabel("📞 联系人页面"));
    stackedWidget->addWidget(contactsPage);

    // 5: 历史
    QWidget *historyPage = new QWidget;
    historyPage->setLayout(new QVBoxLayout);
    historyPage->layout()->addWidget(new QLabel("📜 历史页面"));
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
void MainWindow::onMenuChanged(int index)
{
    if (index < 0 || index >= stackedWidget->count()) {
        qWarning() << "Invalid index:" << index;
        return;
    }

    stackedWidget->setCurrentIndex(index);

    // 保持单选效果
    for (int j = 0; j < buttons.size(); j++)
        buttons[j]->setChecked(j == index);
}

// ============================
// 析构函数
// ============================
MainWindow::~MainWindow() {}
