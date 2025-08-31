#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>

class CardWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CardWidget(const QString &title,
                        const QString &subtitle,
                        QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *e) override;

private:
    QLabel *titleLabel;
    QLabel *subtitleLabel;
};

#endif // CARDWIDGET_H
