#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>


class ClientInfo
{
public:
    //ClientInfo(QString clientName,int clientPort,QString threadId);
    ClientInfo(QString clientName,int clientPort,QString threadId)
        :m_clientName(clientName)
        ,m_clientPort(clientPort)
        ,m_threadId(threadId)
    {}
    QString clientName() const
    {
        return m_clientName;
    }
    void setClientName(const QString &newClientName)
    {
        m_clientName = newClientName;
    }

    int clientPort() const
    {
        return m_clientPort;
    }
    void setClientPort(int newClientPort)
    {
        m_clientPort = newClientPort;
    }

    QString threadId() const
    {
        return m_threadId;
    }
    void setThreadId(const QString &newThreadId)
    {
        m_threadId = newThreadId;
    }

private:
    QString m_clientName;
    int  m_clientPort;
    QString m_threadId;
};

#endif // CLIENTINFO_H
