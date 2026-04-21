#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "login_module.h"

/**
 * @author Joohwan Lim
 * @brief 서버 측 로그인 부분
 * @param id 아이디
 * @param pw 비밀번호
 */
LoginModule::LoginModule(QString id, QString pw)
{
    this->id = id;
    this->pw = pw;
}

/**
 * @author Joohwan Lim
 * @brief 관리자인지 확인 (admin 계정)
 * @return Id가 "admin"이면 true 그렇지 않으면 false
 */
bool LoginModule::isAdmin()
{
    if(this->id == "admin")
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief 로그인을 시도하며 json 정보를 전달.
 * @return Json을 통한 유저 정보
 */
QJsonObject LoginModule::login()
{
    QJsonObject res= findUser();

    if(res["success"].toBool())
    {
        QJsonObject user = res["user"].toObject();

        if(user["id"] == "admin")
        {
            loginWithAdmin();
        }
        else
        {
            loginWithUser();
        }
    }
    return res;
}

void LoginModule::loginWithAdmin()
{
    qDebug() << "admin login";
}

void LoginModule::loginWithUser()
{
    qDebug() << "user login";
}

/**
 * @brief id와 pw에 맞는 유저를 찾는 로직.
 * @return success 여부와 user 정보
 */
QJsonObject LoginModule::findUser()
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

        qDebug() << "id: " << json_id;
        qDebug() << "pw: " << json_pw;
        qDebug() << "your id: " << id;
        qDebug() << "your pw: " << pw;

        if(id == json_id && pw == json_pw)
        {
            QJsonObject res;
            res["success"] = true;
            res["user"] = user;
            return res;
        }
    }

    empty["message"] = "id/pw not matched";
    return empty;
}