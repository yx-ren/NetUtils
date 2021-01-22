#ifndef __NET_UTILS_SERVICES_I_TCP_BROKER_SERVER_ENGINE_H__
#define __NET_UTILS_SERVICES_I_TCP_BROKER_SERVER_ENGINE_H__

#include <NetUtils/common.h>
#include <NetUtils/protocol.h>
#include <NetUtils/services/common.h>
#include <NetUtils/services/IEngine.h>
#include <NetUtils/SocketContext.h>

using NU_NAMESPACE::protocol;

NU_SER_BEGIN

class ITcpBrokerServerEngine : public IEngine
{
public:
    ITcpBrokerServerEngine(const char *tag)
        : IEngine(tag)
    {}

    ITcpBrokerServerEngine(const char *tag, protocol::PROTOCOL_TYPE type);

    ITcpBrokerServerEngine(const char *tag, protocol::PROTOCOL_TYPE type, const std::string& ip, int port);

    virtual ~ITcpBrokerServerEngine() {}

    virtual std::string toString() const override;

    virtual bool start() override;
    virtual void stop() override;
    virtual bool isStarted() const override;

    void enableDumpRequest();
    void disableDumpRequest();
    bool isDumpRequestEnabled() const;

    void setListeningSocket(int fd);
    int getListenSocket() const;

    void setListeningAddress(std::string ip);
    const std::string& getListeningAddress() const;

    void setListeningPort(int port);
    int getListeningPort() const;

    uint32_t getConnections() const;
    uint32_t getMaxConnections() const;

protected:
    virtual bool initIPV4Socket(int& fd);
    virtual bool initIPV6Socket(int& fd);
    virtual bool initListeningSocket();

    virtual bool run() = 0;

protected:
    protocol::PROTOCOL_TYPE mProtocolType;
    std::string mIP;
    int mPort;
    int mFd;
    bool mIsRunning;
    bool mIsDumpRequest;
    int64_t mTotalSendBytes;
    int64_t mTotalRecvBytes;
    uint32_t mConnections;
    uint32_t mMaxConnections;
    mutable CBASE_NAMESPACE::rwmutex mRWLock;
    SocketContextQueue mSocketContextQueue;
};
typedef std::shared_ptr<ITcpBrokerServerEngine> ITcpBrokerServerEngineSptr;
typedef std::weak_ptr<ITcpBrokerServerEngine> ITcpBrokerServerEngineWptr;

NU_SER_END

#endif
