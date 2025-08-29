#include "mainwindow.h"
#include "expertmainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 全局加载 QSS 样式
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QString::fromLatin1(file.readAll());
        a.setStyleSheet(style);
    }

    MainWindow w;
    w.show();

    ExpertMainWindow ew;
    ew.show();

    return a.exec();
}
