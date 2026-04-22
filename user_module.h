#ifndef USER_MODULE_H
#define USER_MODULE_H

#include <QString>
#include <QDate>
class UserModule
{
public:
    UserModule();

    static bool withdraw(QString id);
    static bool verifiedId(QString id);
    static bool signUp(QJsonObject user);

    static QJsonObject findUser(QString id);

private:

    static QJsonDocument readJson();
    static bool writeJson(QJsonDocument);

};

#endif // USER_MODULE_H
