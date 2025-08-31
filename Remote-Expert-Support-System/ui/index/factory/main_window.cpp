#include "main_window.h"
#include "agenda_page.h"
#include "device_page.h"
#include "../../knowledge/data_viewer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QMessageBox>

// ============================
// MainWindow 构造函数
// ============================
MainWindow::MainWindow(Client* client, QWidget *parent)
    : client(client), QMainWindow(parent)
{
    loadStyleSheet();

    setWindowTitle("工厂端");
    setMinimumSize(500, 400);
    resize(1600, 1000);
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // 右边 QStackedWidget (要最先初始化)
    stackedWidget = new QStackedWidget;

    // ================= leftSidebar =================
    QWidget *sidebar = new QWidget;
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(150);
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
        btn->setAutoExclusive(true);  // 单选效果
        btn->setIconSize(QSize(32, 32));
        btn->setMinimumSize(60, 60);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setIcon(QIcon(iconPaths[i]));

        // 关键：默认隐藏文字，用tooltip显示名称
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);  // 只显示图标
        btn->setToolTip(tooltips[i]);  // 鼠标悬停时显示文字提示

        sideLayout->addWidget(btn, 1);
        buttons.append(btn);

        // 绑定按钮点击 → 切换页面
        connect(btn, &QToolButton::clicked, this, [this, i]() {
            onMenuChanged(i);
            onIndexChanged(i);
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
    agendaPage = new AgendaPage;
    stackedWidget->addWidget(agendaPage);

    // 3: 设备
    stackedWidget->addWidget(new DevicePage);

    // 4: 企业知识库
    QWidget *contactsPage = new QWidget;
    contactsPage->setLayout(new QVBoxLayout);
    // 更换为知识库图表
    contactsPage->layout()->addWidget(new DataViewer());
    stackedWidget->addWidget(contactsPage);

    // 5: 历史
    QWidget *historyPage = new QWidget;
    historyPage->setLayout(new QVBoxLayout);
    historyPage->layout()->addWidget(new QLabel("📜 历史页面"));
    stackedWidget->addWidget(historyPage);
    //stackedWidget->addWidget(new HistoryOrder);

    // ================= 右侧联系人侧边栏 =================
    setupContactsSidebar();

    // ================= 主布局 =================
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget, 1); // 中间区域可伸缩
    mainLayout->addWidget(contactsSidebar);  // 右侧固定宽度

    // 默认首页
    stackedWidget->setCurrentIndex(0);
    if (!buttons.isEmpty()) buttons[0]->setChecked(true);

    setCentralWidget(central);

    // 信号槽
    connect(this->client, &Client::agendaResult, this, &MainWindow::onAgendaReceived);
    connect(this->client, &Client::contactResult, this, &MainWindow::onContactReceived);

}

void MainWindow::loadStyleSheet()
{
    QFile styleFile(":/qss/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(style);
        styleFile.close();
        qDebug() << "QSS 文件加载成功！";
    }
    else
    {
        QMessageBox::warning(this, "警告", "无法加载样式表文件！");
    }
}

void MainWindow::setupContactsSidebar()
{
    // 创建右侧联系人侧边栏
    contactsSidebar = new QWidget;
    contactsSidebar->setObjectName("ContactsSidebar");
    contactsSidebar->setFixedWidth(360); // 固定宽度

    QVBoxLayout *sidebarLayout = new QVBoxLayout(contactsSidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // 整个联系人区域容器
    QWidget *contactsContainer = new QWidget;
    contactsContainer->setObjectName("ContactsContainer");
    QVBoxLayout *containerLayout = new QVBoxLayout(contactsContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);

    // 标题栏
    QWidget *titleBar = new QWidget;
    titleBar->setObjectName("ContactsTitleBar");
    titleBar->setFixedHeight(60);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(15, 0, 15, 0);

    QLabel *titleLabel = new QLabel("👥 联系人");
    titleLabel->setObjectName("ContactsTitle");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 24px; color: #333;");

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    // 搜索框
    QWidget *searchBar = new QWidget;
    searchBar->setObjectName("SearchBar");
    searchBar->setFixedHeight(50);
    searchBar->setStyleSheet("background: white;");

    QHBoxLayout *searchLayout = new QHBoxLayout(searchBar);
    searchLayout->setContentsMargins(10, 5, 10, 5);

    QLineEdit *searchInput = new QLineEdit;
    searchInput->setPlaceholderText("🔍 搜索联系人...");
    searchInput->setObjectName("SearchInput");

    searchLayout->addWidget(searchInput);

    // 联系人列表
    contactList = new QListWidget;
    contactList->setObjectName("ContactList");

    // 添加到容器布局
    containerLayout->addWidget(titleBar);
    containerLayout->addWidget(searchBar);
    containerLayout->addWidget(contactList, 1); // 列表可伸缩

    // 将整个联系人容器添加到侧边栏
    sidebarLayout->addWidget(contactsContainer);

    // 连接联系人点击信号
    connect(contactList, &QListWidget::itemClicked, this, &MainWindow::onContactClicked);

    // 连接搜索功能
    connect(searchInput, &QLineEdit::textChanged, this, [this](const QString &text) {
        for (int i = 0; i < contactList->count(); ++i) {
            QListWidgetItem *item = contactList->item(i);
            QString contactName = item->data(Qt::UserRole).toString();
            bool match = contactName.contains(text, Qt::CaseInsensitive);
            contactList->setRowHidden(i, !match && !text.isEmpty());
        }
    });


    qDebug() << "setup Contacts Sidebar";
    // 请求联系人数据
    client->getContact();
}
// ============================
// 联系人点击槽函数
// ============================
void MainWindow::onContactClicked(QListWidgetItem *item)
{
    QString contactName = item->data(Qt::UserRole).toString();

    // 检查是否已经打开了该联系人的聊天窗口
    if (chatDialogs.contains(contactName)) {
        ChatDialog *existingDialog = chatDialogs.value(contactName);
        if (existingDialog) {
            existingDialog->raise();
            existingDialog->activateWindow();
            return;
        }
    }

    // 创建新的聊天窗口
    ChatDialog *chatDialog = new ChatDialog(contactName, this);
    chatDialogs.insert(contactName, chatDialog);

    // 连接关闭信号，用于清理
    connect(chatDialog, &ChatDialog::finished, this, [this, contactName]() {
        chatDialogs.remove(contactName);
    });

    chatDialog->show();
    chatDialog->activateWindow();
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


// 槽函数：根据当前的页面索引判断调用什么数据
void MainWindow::onIndexChanged(int index) {

    // 议程信息
    if(index == 2) {
        client->getAgenda();
    } else {
        ;
    }
}

void MainWindow::onAgendaReceived(bool success, const QVector<Agenda>& agendas, const QString& message)
{
    if (success) {
        // 将议程数据传递给AgendaPage
        if (agendaPage) {
            agendaPage->updateAgendas(agendas);
        }
        statusBar()->showMessage("议程列表已更新，共 " + QString::number(agendas.size()) + " 条记录");
    } else {
        qWarning() << "Failed to get agendas:" << message;
        statusBar()->showMessage("获取议程列表失败: " + message);
        QMessageBox::warning(this, "错误", "获取议程列表失败: " + message);
    }
}

// 添加处理联系人数据的槽函数
void MainWindow::onContactReceived(bool success, const QVector<User> &contacts, const QString& messages)
{
    qDebug() << "onContactReceived";
    if(success) {
        rawContacts = contacts;
        contactList->clear();
        // 填充联系人列表
        for (const User &contact : rawContacts) {
            QListWidgetItem *item = new QListWidgetItem(contactList);

            // 创建自定义的联系人项widget
            QWidget *itemWidget = new QWidget;
            itemWidget->setFixedHeight(90);
            itemWidget->setStyleSheet("background: white;");

            QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
            itemLayout->setContentsMargins(15, 5, 15, 5);
            itemLayout->setSpacing(10);

            // 头像（使用首字母）
            QLabel *avatarLabel = new QLabel;
            avatarLabel->setFixedSize(45, 45);
            avatarLabel->setStyleSheet(QString(
                                           "border-radius: 20px; background-color: #%1; "
                                           "color: white; font-weight: bold; font-size: 16px;"
                                           ).arg(QString::number(qHash(contact.account) % 0xFFFFFF, 16), 6, QLatin1Char('0')));
            avatarLabel->setAlignment(Qt::AlignCenter);
            avatarLabel->setText(contact.account.left(1).toUpper());

            // 姓名和职位
            QWidget *infoWidget = new QWidget;
            infoWidget->setStyleSheet("background: transparent;");
            QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
            infoLayout->setContentsMargins(0, 0, 0, 0);
            infoLayout->setSpacing(2);

            QLabel *nameLabel = new QLabel(contact.account);
            nameLabel->setObjectName("ContactName");
            nameLabel->setStyleSheet("font-weight: 500; font-size: 18px; color: #333;");

            QLabel *positionLabel = new QLabel(contact.expertSkill);
            positionLabel->setObjectName("ContactPosition");
            positionLabel->setStyleSheet("color: #666; font-size: 15px;");

            infoLayout->addWidget(nameLabel);
            infoLayout->addWidget(positionLabel);

            itemLayout->addWidget(avatarLabel);
            itemLayout->addWidget(infoWidget, 1);
            itemLayout->addStretch();

            itemWidget->setLayout(itemLayout);
            item->setSizeHint(QSize(0, 90));
            contactList->setItemWidget(item, itemWidget);

            // 存储联系人信息
            item->setData(Qt::UserRole, contact.account);
        }

        qDebug() << "Contacts loaded:" << contacts.size();
    }
}




// ============================
// 析构函数
// ============================
MainWindow::~MainWindow() {}
