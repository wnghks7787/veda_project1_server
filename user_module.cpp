#include "user_module.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

UserModule::UserModule() {}

bool UserModule::withdraw(QString id)
{
    qDebug() << "delete start: " << id;
    QFile file("user.json");

    if(!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isArray())
        return false;

    QJsonArray users = doc.array();

    bool found = false;

    for(int i = 0 ; i < users.size() ; i++)
    {
        QJsonObject user = users[i].toObject();

        if(user["id"].toString() == id)
        {
            users.removeAt(i);
            found = true;
            break;
        }
    }

    if(!found)
        return false;

    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(QJsonDocument(users).toJson(QJsonDocument::Indented));
    file.close();

    return true;
}