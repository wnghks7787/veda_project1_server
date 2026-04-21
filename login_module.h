#ifndef LOGIN_MODULE_H
#define LOGIN_MODULE_H

#include <QString>
#include <QJsonObject>

class LoginModule
{
public:
    LoginModule(QString id, QString pw);

    bool isAdmin();

    QJsonObject login();
    QJsonObject findUser();

private:
    QString id;
    QString pw;

    void loginWithAdmin();
    void loginWithUser();

    bool checkLoginValidate();
};

#endif // LOGIN_MODULE_H
