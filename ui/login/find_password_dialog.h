#ifndef FIND_PASSWORD_DIALOG_H
#define FIND_PASSWORD_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

/* 可以暂时先留着，其他写完了再写 */
class FindPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindPasswordDialog(QWidget *parent = nullptr);

signals:
    void findRequested(const QString &emailOrPhone);   // 找回请求信号

private slots:
    void onFindClicked();
    void onCancelClicked();

private:
    void setupUI();

    QLineEdit   *inputEdit;
    QLineEdit   *verifyEdit;
    QPushButton *findBtn;
    QPushButton *cancelBtn;
};

#endif // FIND_PASSWORD_DIALOG_H
