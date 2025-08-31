#ifndef CONFERENCE_TEST_H
#define CONFERENCE_TEST_H
#include <QApplication>
#include "../ui/conference/conference_ui.h"

int conferenceTest(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConferenceUi w;
    w.show();
    return a.exec();
}
#endif // CONFERENCE_TEST_H
