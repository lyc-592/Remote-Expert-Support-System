#ifndef AGENDAPAGE_H
#define AGENDAPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include <QPair>
#include "client/core/client.h"

class AgendaPage : public QWidget
{
    Q_OBJECT
public:
    explicit AgendaPage(Client *client, QWidget *parent = nullptr);


signals:
    // 提交时可以发一个信号给外部
    void agendaCreated(const QString &title, const QString &desc);

private slots:
    void onAddAgenda();     // ➕ 点击
    void onRemoveAgenda();  // ➖ 点击

private:
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;
    QTableWidget *agendaTable;

    Client *client;
    QList<QPair<int, QString>> experts;
    QList<QPair<int, QString>> devices;
};

#endif // AGENDAPAGE_H
