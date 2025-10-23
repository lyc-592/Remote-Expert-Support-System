// mainconferencewindow.h
#ifndef MAINCONFERENCEWINDOW_H
#define MAINCONFERENCEWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "conferencewindow.h"
#include "conference_ui.h"

class MainConferenceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainConferenceWindow(QWidget *parent = nullptr);
    ~MainConferenceWindow();

private:
    void setupUI();
    // void connectSignals();

    ConferenceWindow *conferenceWindow;
    ConferenceUi *conferenceUi;
    QSplitter *mainSplitter;
};

#endif // MAINCONFERENCEWINDOW_H
