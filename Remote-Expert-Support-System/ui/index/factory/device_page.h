#ifndef DEVICE_PAGE_H
#define DEVICE_PAGE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DevicePage : public QWidget
{
    Q_OBJECT
public:
    explicit DevicePage(QWidget *parent = nullptr);

private slots:
    void onAddDevice();
    void onRemoveDevice();
    void onEditDevice();
    void onViewDevice();

private:
    QListWidget *deviceList;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
};

#endif // DEVICE_PAGE_H
