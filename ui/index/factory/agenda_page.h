#ifndef AGENDA_PAGE_H
#define AGENDA_PAGE_H
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include "../../server/entities/agenda.h"
#include "../../server/entities/user.h"

class AgendaPage : public QWidget
{
    Q_OBJECT
public:
    explicit AgendaPage(QWidget *parent = nullptr);
    void updateAgendas(const QVector<Agenda>& agendas);

public slots:
    void onAddAgendaReceived(const QVector<User>& expertList);
signals:
    // 提交时可以发一个信号给外部（比如 MainWindow / 专家端）
    void addAgendaSignal(const QString& title, const QString& device, const QString& startDate,
                         const QString& endDate, const QString& faultDesc,
                         const QString& status, const QString& expertname);
    void requestExpertList();

private slots:
    void onAddAgenda();     // ➕ 点击
    void onRemoveAgenda();  // ➖ 点击

private:
    void attendOrder();

    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;
    QTableWidget *agendaTable;

};

#endif // AGENDA_PAGE_H
