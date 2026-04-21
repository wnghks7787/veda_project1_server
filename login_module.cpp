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

bool LoginModule::isUser()
{
    return true;
}

int LoginModule::login()
{
    if(checkLoginValidate())
    {
        if(isAdmin())
        {
            loginWithAdmin();
            return 1;
        }
        else if(isUser())
        {
            loginWithUser();
            return 2;
        }
    }

    return -1;
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
 * @brief 로그인이 가능한지 확인하는 로직.
 * @return 로그인 정보가 있으면 true, 없으면 false 반환
 */
bool LoginModule::checkLoginValidate()
{
    QFile file("user.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "파일 열기 실패";
    }

    QByteArray data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray users = doc.array();

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
            return true;
        }
    }
    return false;
}