#ifndef ADD_AGENDA_DIALOG_H
#define ADD_AGENDA_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>

class AddAgendaDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddAgendaDialog(QWidget *parent = nullptr);

    QString getTitle() const { return titleEdit->text().trimmed(); }
    QString getDevice() const { return deviceEdit->text().trimmed(); }
    QDate getStartDate() const { return startDateEdit->date(); }
    QDate getEndDate() const { return endDateEdit->date(); }
    QString getFaultDesc() const { return faultEdit->text().trimmed(); }
    QString getExpert() const { return expertBox->currentText(); }
    QString getStatus() const { return statusLabel->text(); }

private slots:
    void onAccept();
    void updateStatus();

private:
    QLineEdit *titleEdit;
    QLineEdit *deviceEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QLineEdit *faultEdit;
    QLabel *statusLabel;
    QComboBox *expertBox;

    QPushButton *btnOk;
    QPushButton *btnCancel;
};

#endif // ADD_AGENDA_DIALOG_H
