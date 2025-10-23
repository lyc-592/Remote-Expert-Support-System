#ifndef USER_PAGE_H
#define USER_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QJsonObject>
#include <QPushButton>
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
    void logoutRequested();
    void editModeChanged(bool enabled);

private:
    void setupUI();
    void toggleEditMode(bool enabled);

    QLabel *userIdLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *logoutBtn;
    QPushButton *editToggleButton;

    bool isEditMode;
    User user;
};

#endif // USER_PAGE_H
