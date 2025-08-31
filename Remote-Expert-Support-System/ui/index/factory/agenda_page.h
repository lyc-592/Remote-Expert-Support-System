#ifndef AGENDA_PAGE_H
#define AGENDA_PAGE_H
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include "../../server/entities/agenda.h"

class AgendaPage : public QWidget
{
    Q_OBJECT
public:
    explicit AgendaPage(QWidget *parent = nullptr);
    void updateAgendas(const QVector<Agenda>& agendas);

signals:
    // 提交时可以发一个信号给外部（比如 MainWindow / 专家端）
    void agendaCreated(const QString &title, const QString &desc);

private slots:
    void onAddAgenda();     // ➕ 点击
    void onRemoveAgenda();  // ➖ 点击

private:
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;
    QTableWidget *agendaTable;
};

#endif // AGENDA_PAGE_H
