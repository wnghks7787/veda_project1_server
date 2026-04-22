#include "user_module.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

UserModule::UserModule() {}

/**
 * @brief json 파일을 읽기
 * @return
 */
QJsonDocument UserModule::readJson()
{
    QFile file("user.json");

    if(!file.open(QIODevice::ReadOnly))
    {
        return QJsonDocument();
    }

    QByteArray data = file.readAll();
    file.close();

    return QJsonDocument::fromJson(data);
}

/**
 * @brief json 파일을 쓰기
 * @param doc 적을 json 문서
 * @return 성공시 true
 */
bool UserModule::writeJson(QJsonDocument doc)
{
    QFile file("user.json");

    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.write(QJsonDocument(doc).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

/**
 * @brief 서버 회원탈퇴 로직
 * @param id widhdraw하기 위한 아이디
 * @return 성공하면 true
 */
bool UserModule::withdraw(QString id)
{
    QJsonDocument doc = readJson();

    if(doc.isNull() || !doc.isArray())
    {
        return false;
    }

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

    return writeJson(QJsonDocument(users));
}

/**
 * @brief 서버 아이디 중복 확인 로직
 * @param id 확인할 아이디
 * @return 성공시 true
 */
bool UserModule::verifiedId(QString id)
{
    QJsonObject user = findUser(id);
    if(!user["success"].toBool())
    {
        return true;
    }

    return false;

}

/**
 * @brief 서버 회원가입 로직
 * @param user 회원 가입을 할 유저의 정보
 * @return
 */
bool UserModule::signUp(QJsonObject user)
{
    QString name = user["name"].toString();
    QDate birthday = QDate::fromString(user["birthday"].toString(), Qt::ISODate);
    QString id = user["id"].toString();
    QString password = user["password"].toString();
    QString phoneNum = user["phoneNum"].toString();
    QString note = user["note"].toString();

    QJsonDocument doc = readJson();

    if(doc.isNull() || !doc.isArray())
    {
        return false;
    }

    QJsonArray users;
    users = doc.array();

    QJsonObject obj;

    obj["id"]=  id;
    obj["note"] = note;

    QJsonObject infoObj;
    infoObj["name"] = name;
    infoObj["birthday"] = birthday.toString("yyyy-MM-dd");
    infoObj["password"] = password;
    infoObj["phoneNum"] = phoneNum;
    infoObj["age"] = QDate::currentDate().year() - birthday.year() + 1;

    obj["info"] = infoObj;


    QJsonObject attendanceObj;
    attendanceObj["present"] = 0;
    attendanceObj["absent"] = 0;
    attendanceObj["late"] = 0;
    attendanceObj["earlyLeave"] = 0;
    attendanceObj["beOut"] = 0;

    obj["attendance"] = attendanceObj;

    users.append(obj);

    return writeJson(QJsonDocument(users));
}

/**
 * @brief 서버 유저 정보 수정 로직
 * @param user 수정될 유저 정보
 * @return 성공시 true
 */
bool UserModule::editUser(QJsonObject updatedUser)
{
    QJsonDocument doc = readJson();

    if(doc.isNull() || !doc.isArray())
    {
        return false;
    }

    QJsonArray users = doc.array();
    QString targetId = updatedUser["id"].toString();
    bool found = false;

    for(int i = 0; i < users.size(); i++)
    {
        QJsonObject user = users[i].toObject();
        if(user["id"].toString() == targetId)
        {
            users.replace(i, updatedUser);
            found = true;
            break;
        }
    }

    if(!found)
    {
        return false;
    }

    return writeJson(QJsonDocument(users));
}

/**
 * @brief 유저를 찾는 로직.
 * @param id 찾을 유저의 id
 * @return 찾으면 true
 */
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
    // Admin일 경우 전체 유저를 보내줌
    for(const QJsonValue &value: users)
    {
        QJsonObject user = value.toObject();

        QString jsonId = user["id"].toString();
        QString jsonPw = user["info"].toObject()["password"].toString();


        if(id == jsonId)
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
