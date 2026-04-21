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
    static void signUp(
        QString name,
        QDate birthday,
        QString id,
        QString password,
        QString phone_num);

    static QJsonObject findUser(QString id);

private:

};

#endif // USER_MODULE_H
