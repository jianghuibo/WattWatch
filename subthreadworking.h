#ifndef SUBTHREADWORKING_H
#define SUBTHREADWORKING_H

#include <QObject>
#include<QRunnable>
#include<QThread>
#include<QTcpSocket>
#include <QEventLoop>
class subThreadWorking : public QObject,public QRunnable
{
    Q_OBJECT
public:
    subThreadWorking(qintptr socketDescriptor)
        :m_socketDescriptor(socketDescriptor){}

    /*
    当有一个客户端连接的时候，就执行run函数
    用来接受客户端发送的数据
    */
    void run()override
    {
        //qDebug()<<"子线程ID："<<QThread::currentThreadId();
        QString threadIdString = QString::number(reinterpret_cast<uintptr_t>(QThread::currentThreadId()));
        emit sendThreadId(threadIdString);
        QTcpSocket *clientSocket = new QTcpSocket();
        clientSocket->setSocketDescriptor(m_socketDescriptor);
        first = 0;
        connect(clientSocket, &QTcpSocket::readyRead, clientSocket, [=]() {
            if(first == 0)
            {
                QByteArray data = clientSocket->readAll();
                emit sendName(data);
                first++;
            }
            else{
                QByteArray data = clientSocket->readAll();
                emit sendReceived(data);
            }
        });
        QEventLoop eventLoop;
        connect(clientSocket,&QTcpSocket::disconnected,&eventLoop,&QEventLoop::quit);
        eventLoop.exec();
        clientSocket->disconnectFromHost();
    }

signals:
    void sendReceived(QByteArray data);
    void sendThreadId(QString data);
    void sendName(QByteArray name);

private:
    qintptr m_socketDescriptor;     //套接字描述符
    int first;                      //用来接受第一个数据，（第一个数据是客户端的名称）
};

#endif // SUBTHREADWORKING_H
