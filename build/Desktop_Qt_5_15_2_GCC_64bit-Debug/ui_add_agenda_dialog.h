/********************************************************************************
** Form generated from reading UI file 'add_agenda_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_AGENDA_DIALOG_H
#define UI_ADD_AGENDA_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_AgendaDialog
{
public:

    void setupUi(QDialog *AgendaDialog)
    {
        if (AgendaDialog->objectName().isEmpty())
            AgendaDialog->setObjectName(QString::fromUtf8("AgendaDialog"));
        AgendaDialog->resize(400, 300);

        retranslateUi(AgendaDialog);

        QMetaObject::connectSlotsByName(AgendaDialog);
    } // setupUi

    void retranslateUi(QDialog *AgendaDialog)
    {
        AgendaDialog->setWindowTitle(QCoreApplication::translate("AgendaDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AgendaDialog: public Ui_AgendaDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_AGENDA_DIALOG_H
