#include "server.h"

#include "login_module.h"
#include "user_module.h"
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

Server::Server(QObject* parent)
    : QTcpServer(parent)
{

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);

    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qDebug() << "클라이언트 연결됨";
}

bool Server::startServer(int port)
{
    if(this->listen(QHostAddress::Any, port))
    {
        qDebug() << "서버 시작 성공, port: " << port;
        return true;
    }
    else
    {
        qDebug() << "서버 시작 실패";
        return false;
    }
}

void Server::onReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();

    QByteArray data = socket->readAll();

    qDebug() << "받은 데이터: " << data;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QString type = obj["type"].toString();

    QJsonObject response;

    if(type == "login")
    {
        QJsonObject login_info = onLogin(obj);

        response["type"] = "login";
        if(login_info["success"].toBool())
        {
            response["success"] = true;
            response["user"] = login_info["user"];

            if(response["user"].toObject()["id"].toString() == "admin")
            {
                response["users_info"] = login_info["users_info"];
            }
        }
        else
        {
            response["success"] = false;
        }
    }
    else if(type == "withdraw")
    {
        qDebug() << "withdraw";
        UserModule::withdraw(obj["id"].toString());
    }
    else if(type == "verifyId")
    {
        qDebug() << "verifyId";
        bool res = UserModule::verifiedId(obj["id"].toString());

        response["type"] = "verifyId";
        if(res)
        {
            response["success"] = true;
        }
        else
        {
            response["success"] = false;
        }
    }
    else if(type == "signUp")
    {
        qDebug() << "signUp";
        bool res = UserModule::signUp(obj["user"].toObject());

        response["type"] = "signUp";
        if(res)
        {
            response["success"] = true;
        }
        else
        {
            response["success"] = false;
        }
    }

    socket->write(QJsonDocument(response).toJson());
}

QJsonObject Server::onLogin(QJsonObject obj)
{
    QString id = obj["id"].toString();
    QString pw = obj["password"].toString();

    LoginModule loginModule(id, pw);
    return loginModule.login();
}

void Server::onDisconnected()
{
    qDebug() << "종료";
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}