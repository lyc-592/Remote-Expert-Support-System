#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "../../client/core/client.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();
    void onLoginResult(bool success, const QString& message);
    void onCancelClicked();
    void openRegisterDialog();
    void onUserRegistered(const QString &user, const QString &pwd,
                          UserType userType, const QString &contact,
                          const QString &expertSkill);
    void onRegisterResult(bool success, const QString &message);
    void openMainWindow(bool success, const QString& message, const User& userData);
signals:
    void registerSignal(bool success, const QString& message);

private:
    void setupUI();
    void loadStyleSheet();

    Client *client;
    QScrollArea *scrollArea;
    QLabel *titleLabel;
    QLabel *userLabel;
    QLineEdit *userEdit;
    QLabel *passLabel;
    QLineEdit *passEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLabel *registerLabel;
    QPushButton *registerButton;
};

#endif // LOGIN_WINDOW_H
