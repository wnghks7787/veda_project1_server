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

bool UserModule::verifiedId(QString id)
{
    QJsonObject user = findUser(id);
    if(!user["success"].toBool())
    {
        return true;
    }

    return false;

}

void UserModule::signUp(QString name, QDate birthday, QString id, QString password, QString phone_num)
{
    QFile file("user.json");
    QJsonArray users;

    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);

        if(doc.isArray())
        {
            users = doc.array();
        }
    }

    QJsonObject obj;
    obj["id"]=  id;

    QJsonObject infoObj;
    infoObj["name"] = name;
    infoObj["birthday"] = birthday.toString("yyyy-MM-dd");
    infoObj["password"] = password;
    infoObj["phone_num"] = phone_num;
    infoObj["age"] = QDate::currentDate().year() - birthday.year() + 1;

    obj["info"] = infoObj;


    QJsonObject attendanceObj;
    attendanceObj["present"] = 0;
    attendanceObj["absent"] = 0;
    attendanceObj["late"] = 0;
    attendanceObj["early_leave"] = 0;
    attendanceObj["be_out"] = 0;

    obj["attendance"] = attendanceObj;

    users.append(obj);

    if(file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(users).toJson(QJsonDocument::Indented));
        file.close();
    }
}

QJsonObject UserModule::findUser(QString id)
{
    QJsonObject empty;
    empty["success"] = false;

    QFile file("user.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "파일 열기 실패";
        return empty;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray users = doc.array();


    // 각각을 돌며 유저를 찾는 로직
    // Admin일 경우 특정 값을 더 보내주도록 하자.
    for(const QJsonValue &value: users)
    {
        QJsonObject user = value.toObject();

        QString json_id = user["id"].toString();
        QString json_pw = user["info"].toObject()["password"].toString();


        if(id == json_id)
        {
            QJsonObject res;
            res["success"] = true;
            res["user"] = user;
            return res;
        }
    }

    empty["message"] = "id not matched";
    return empty;
}
