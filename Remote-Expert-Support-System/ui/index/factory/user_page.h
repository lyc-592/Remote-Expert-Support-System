#ifndef USER_PAGE_H
#define USER_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QJsonObject>
#include "../../server/entities/user.h"

class UserPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserPage(User& user, QWidget *parent = nullptr);
public slots:
    void refresh(User& user);
private slots:
    void saveUserData();
signals:
    void updateUser(const QString& username, const QString& password);

private:
    void setupUI();


    QLabel *userIdLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;

    User user;
};

#endif // USER_PAGE_H
