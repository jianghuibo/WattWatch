#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include<QTcpServer>
#include<QThreadPool>
#include"subthreadworking.h"
#include"clientinfo.h"
class ThreadPool : public QObject
{
    Q_OBJECT
public:
    explicit ThreadPool(QObject *parent = nullptr);
    void start();
    void paddingClientInfo(QTcpSocket *tcpsocket,subThreadWorking *sub);
    void disconnectClient(int port);

private slots:
    void handleConnection();
    void handleData(QByteArray data);

signals:
    void sendClintInfo(ClientInfo clientInfo);

private:
    QTcpServer *m_tcpserver;                    //服务器
    int m_clientPort;                           //连接的客户端的端口号
    QString m_threadId;                         //连接的客户端每次打开的线程的线程ID
    QString m_clientName;                       //连接的客户端的客户端名称
    QMap<int,QTcpSocket*>m_clientConnections;   //用来存储每一个客户端的端口：TcpSocket
signals:
};

#endif // THREADPOOL_H
