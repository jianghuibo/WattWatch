#include "threadpool.h"
#include"subthreadworking.h"
ThreadPool::ThreadPool(QObject *parent)
    : QObject{parent}
    ,m_tcpserver(NULL)
{}

/*
    启动服务器
*/
void ThreadPool::start()
{
    m_tcpserver = new QTcpServer();
    m_tcpserver->listen(QHostAddress::Any,8989);
    connect(m_tcpserver,&QTcpServer::newConnection,this,&ThreadPool::handleConnection);
}

/*
    将与服务器连接的客户端的信息（客户端名称，端口号，线程ID），组装成ClientInfo,并将组装好
    的ClientInfo发送出去
*/
void ThreadPool::paddingClientInfo(QTcpSocket *tcpsocket,subThreadWorking *sub)
{
    m_clientPort = tcpsocket->peerPort();
    connect(sub,&subThreadWorking::sendThreadId,this,[&](QString threadId){
        m_threadId = threadId;
        //qDebug()<<m_clientPort<<" "<<m_threadId;
    });
    connect(sub,&subThreadWorking::sendName,this,[&](QByteArray data){
        m_clientName = data;
        qDebug()<<m_clientPort<<" "<<m_threadId<<" "<<m_clientName;
        ClientInfo clientInfo(m_clientName,m_clientPort,m_threadId);
        emit sendClintInfo(clientInfo);
    });
}

/*
    将删除的客户端与服务器断开连接
*/
void ThreadPool::disconnectClient(int port)
{
    QTcpSocket *clientSocket = m_clientConnections.value(port);
    clientSocket->disconnectFromHost();
    m_clientConnections.remove(port);
    qDebug()<<"删除后:"<<m_clientConnections.size();
}

/*
    当有客户端连接的时候
*/
void ThreadPool::handleConnection()
{
    while(m_tcpserver->hasPendingConnections())
    {
        QTcpSocket *clientSocket = m_tcpserver->nextPendingConnection();
        subThreadWorking *sub = new subThreadWorking(clientSocket->socketDescriptor());

        QThreadPool::globalInstance()->start(sub);
        paddingClientInfo(clientSocket,sub);
        m_clientConnections.insert(clientSocket->peerPort(),clientSocket);
        qDebug()<<"map:"<<m_clientConnections.size();
        connect(sub, &subThreadWorking::sendReceived, this, &ThreadPool::handleData);
    }
}

/*
    对客户端发送过来的数据进行处理
*/
void ThreadPool::handleData(QByteArray data)
{
    qDebug()<<"接受到的数据："<<data;
}
