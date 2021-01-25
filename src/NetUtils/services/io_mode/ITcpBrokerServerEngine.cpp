#include <NetUtils/services/io_mode/ITcpBrokerServerEngine.h>
#include <functional>
#include <map>
#include <common/base/logger/LoggerManager.h>

#define BACK_LOG                0xff
using namespace CBASE_LOGGER_NAMESPACE;

NU_SER_BEGIN


ITcpBrokerServerEngine::ITcpBrokerServerEngine(const char *tag, protocol::PROTOCOL_TYPE type)
    : IEngine(tag)
    , mProtocolType(type)
    , mIP("")
    , mPort(0)
    , mFd(INVALID_FD)
    , mIsRunning(false)
    , mIsDumpRequest(false)
    , mTotalSendBytes(0)
    , mTotalRecvBytes(0)
    , mConnections(0)
{}

ITcpBrokerServerEngine::ITcpBrokerServerEngine(const char *tag, protocol::PROTOCOL_TYPE type, const std::string& ip, int port)
    : IEngine(tag)
    , mProtocolType(type)
    , mIP(ip)
    , mPort(port)
    , mFd(INVALID_FD)
    , mIsRunning(false)
    , mIsDumpRequest(false)
    , mTotalSendBytes(0)
    , mTotalRecvBytes(0)
    , mConnections(0)
{}

std::string ITcpBrokerServerEngine::toString() const
{
    std::ostringstream oss;
    oss << "name:" << getTag()
        << "; ip:" << mIP
        << "; port:" << mPort
        << "; fd:" << mFd
        << "; protocol:" << protocol::toString(mProtocolType)
        ;

    return oss.str();
}

bool ITcpBrokerServerEngine::start()
{
    if (mFd == INVALID_FD)
    {
        if (!initListeningSocket())
        {
            CBT_ERROR("ITcpBrokerServerEngine", "start() faild to init listening socket, "
                    "engine: " << toString() << " not started");
            return false;
        }
    }
    
    if (!run())
    {
        CBT_ERROR("ITcpBrokerServerEngine", "start() faild to run engine:" << toString());
        return false;
    }

    return true;
}

void ITcpBrokerServerEngine::stop()
{
    // TODO......
    if (mFd != INVALID_FD)
        close(mFd);

    return;
}

bool ITcpBrokerServerEngine::isStarted() const
{
    return mIsRunning;
}

void ITcpBrokerServerEngine::enableDumpRequest()
{
    mIsDumpRequest = true;
}

void ITcpBrokerServerEngine::disableDumpRequest()
{
    mIsDumpRequest = false;
}

bool ITcpBrokerServerEngine::isDumpRequestEnabled() const
{
    return mIsDumpRequest;
}

void ITcpBrokerServerEngine::setListeningSocket(int fd)
{
    if (mFd != INVALID_FD)
        mFd = fd;
}

int ITcpBrokerServerEngine::getListenSocket() const
{
    return mFd;
}

void ITcpBrokerServerEngine::setListeningAddress(std::string ip)
{
    mIP = ip;
}

const std::string& ITcpBrokerServerEngine::getListeningAddress() const
{
    return mIP;
}

void ITcpBrokerServerEngine::setListeningPort(int port)
{
    mPort = port;
}

int ITcpBrokerServerEngine::getListeningPort() const
{
    return mPort;
}

uint32_t ITcpBrokerServerEngine::getConnections() const
{
    return mConnections;
}

uint32_t ITcpBrokerServerEngine::getMaxConnections() const
{
    return mMaxConnections;
}

void ITcpBrokerServerEngine::regiesterNewClientCB(handelNewClient cb)
{
    mHandleNewClientCb = cb;
}

bool ITcpBrokerServerEngine::initIPV4Socket(int& fd)
{
#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV4Socket() faild to win32 net env");
        return false;
    }
#endif

    // create server socket
    int svr_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_sock < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV4Socket() create socket failed, err code:" << errno);
        return false;
    }

#if 1
    int option = 1;
    if (setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV4Socket() set sockect option:SO_REUSEADDR failed, "
            << "err code:" << errno);
        return false;
    }
    // read more argument from xml config
    // TODO......
#endif

    CBT_DEBUG("ITcpBrokerServerEngine", "initIPV4Socket() create ipv4 tcp server socket......");

    // -------------------- //

    // init sockaddr_in struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = mIP.empty() ? INADDR_ANY : inet_addr(mIP.c_str());
    server_addr.sin_port        = htons(mPort);

    // bind
    if (bind(svr_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV4Socket() failed bind the socket to "
                << "ip:" << mIP << "port:" << mPort << ", "
                << "error code:" << errno);
        return false;
    }

    CBT_DEBUG("ITcpBrokerServerEngine", "initIPV4Socket() bind the local address, "
        << ", ip:" << (mIP.empty() ? "127.0.0.1" : mIP)
        << ", port:" << mPort);

    // listen
    if (listen(svr_sock, BACK_LOG) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV4Socket call listen() failed, "
            << ", listen port:" << mPort
            << ", error code:" << errno);
        return false;
    }

    mFd = svr_sock;

    return true;
}

bool ITcpBrokerServerEngine::initIPV6Socket(int& fd)
{
#ifdef WIN32
    // init win32 socket
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() faild to win32 net env");
        return false;
    }
#endif

    // create server socket
    int svr_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (svr_sock < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() create socket failed, err code:" << errno);
        return false;
    }

#if 1
    int option = 1;
    if (setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() set sockect option:SO_REUSEADDR failed, "
            << "err code:" << errno);
        return false;
    }

    if (setsockopt(svr_sock, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&option, sizeof(option)) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() set sockect option:IPV6_V6ONLY failed, "
            << "err code:" << errno);
        return false;
    }
    // read more argument from xml config
    // TODO......
#endif

    CBT_DEBUG("ITcpBrokerServerEngine", "initIPV6Socket() create ipv6 tcp server socket......");

    // init sockaddr_in struct
    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family      = AF_INET6;
    server_addr.sin6_port        = htons(mPort);
    if (mIP.empty())
        server_addr.sin6_addr = in6addr_any;
    else
    {
        if (inet_pton(AF_INET6, mIP.c_str(), &server_addr.sin6_addr) != 1)
        {
            CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() ip:" << mIP << " not a valid address, "
                    << "error code:" << errno);
            return false;
        }
    }

    // bind
    if (bind(svr_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket() failed bind the socket to "
                << "ip:" << mIP << "port:" << mPort << ", "
                << "error code:" << errno);
        return false;
    }

    CBT_DEBUG("ITcpBrokerServerEngine", "initIPV6Socket() bind the local address, "
        << ", ip:" << (mIP.empty() ? "::1" : mIP)
        << ", port:" << mPort);

    // listen
    if (listen(svr_sock, BACK_LOG) < 0)
    {
        CBT_WARN("ITcpBrokerServerEngine", "initIPV6Socket call listen() failed, "
            << ", listen port:" << mPort
            << ", error code:" << errno);
        return false;
    }

    mFd = svr_sock;

    return true;
}

bool ITcpBrokerServerEngine::initListeningSocket()
{
    std::map<protocol::PROTOCOL_TYPE, std::function<bool()>> init_funs =
    {
        {protocol::PROTOCOL_TYPE::PT_TCP_IPV4, std::bind(&ITcpBrokerServerEngine::initIPV4Socket, this, std::ref(mFd))},
        {protocol::PROTOCOL_TYPE::PT_TCP_IPV6, std::bind(&ITcpBrokerServerEngine::initIPV6Socket, this, std::ref(mFd))},
    };

    auto init_fun = init_funs.find(mProtocolType);
    if (init_fun == init_funs.end())
    {
        CBT_WARN("ITcpBrokerServerEngine", "initListeningSocket() "
                "engine:" << toString() << " not found the init handle");
        return false;
    }

    if (!init_fun->second())
        return false;

    return true;
}

NU_SER_END
