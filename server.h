#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <QJsonObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);

    bool startServer(int port = 12345);

    QJsonObject onLogin(QJsonObject obj);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:

private slots:
    void onReadyRead();
    void onDisconnected();


};

#endif // SERVER_H
