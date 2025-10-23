#ifndef EXPERT_AGENDA_PAGE_H
#define EXPERT_AGENDA_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include "../../server/entities/agenda.h"
#include "../../server/entities/user.h"

class ExpertAgendaPage : public QWidget
{
    Q_OBJECT
public:
    explicit ExpertAgendaPage(QWidget *parent = nullptr);
    void updateAgendas(const QVector<Agenda>& agendas);
    void initialize();

signals:
    void requestGetAgenda();

private:
    void attendOrder();

    void setupUI();
    QPushButton *btnJoin;
    QTableWidget *agendaTable;

};


#endif // EXPERT_AGENDA_PAGE_H
