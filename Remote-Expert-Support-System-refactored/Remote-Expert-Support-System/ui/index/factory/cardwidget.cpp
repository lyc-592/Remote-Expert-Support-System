#include "cardwidget.h"

CardWidget::CardWidget(const QString &title,
                       const QString &subtitle,
                       QWidget *parent)
    : QFrame(parent)
{
    // 去掉焦点和边框
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QFrame { "
                  "background-color: #EDE7F6;"   // 默认浅紫
                  "border-radius: 16px;"         // 圆角
                  "border: none;"                // 无边框
                  "}"
                  "QFrame:hover { "
                  "background-color: #D1C4E9;"   // hover 更深
                  "}");

    // 自适应布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // 标题（比如 "议程"）
    titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 17px; font-weight: bold; color: #5E35B1;");

    // 副标题（比如 "查看最新议程"）
    subtitleLabel = new QLabel(subtitle, this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 13px; color: #7E57C2;");

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addStretch();

    setLayout(layout);

    // 让卡片横向铺满，高度固定
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(100);
}

void CardWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit clicked();
    }
    QFrame::mousePressEvent(e);
}
