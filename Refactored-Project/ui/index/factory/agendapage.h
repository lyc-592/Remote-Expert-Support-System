#ifndef AGENDAPAGE_H
#define AGENDAPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>

class AgendaPage : public QWidget
{
    Q_OBJECT
public:
    explicit AgendaPage(QWidget *parent = nullptr);

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

#endif // AGENDAPAGE_H
