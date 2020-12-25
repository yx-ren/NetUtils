#include <async_client/AsyncClient.h>

AsyncClient::AsyncClient(std::string ip, uint16_t port)
{
    mImpl = std::shared_ptr<AsyncClientImpl>(new AsyncClientImpl(ip, port));
    if (!mImpl)
        throw std::runtime_error("call AsyncClientImpl() construction failed");
}

AsyncClient::~AsyncClient()
{
    mImpl.reset();
}

void AsyncClient::registerConnectedCB(connectedCallback conn_cb)
{
    return mImpl->registerConnectedCB(conn_cb);
}

void AsyncClient::registerReadCB(readCallback read_cb)
{
    return mImpl->registerReadCB(read_cb);
}

void AsyncClient::registerWriteCB(writeCallback write_cb)
{
    return mImpl->registerWriteCB(write_cb);
}

void AsyncClient::connect()
{
    return mImpl->connect();
}

class AsyncClientImpl
{
    AsyncClientImpl(std::string ip, uint16_t port)
        : mServerIP(ip)
        , mServerPort(port)

    ~AsyncClientImpl()
    {}

    void registerConnectedCB(connectedCallback conn_cb);
    void registerReadCB(readCallback read_cb);
    void registerWriteCB(writeCallback write_cb);

    void connect();

private:
    std::string mServerIP;
    uint16_t mServerPort;

    connectedCallback mConnCb;
    readCallback mReadCb;
    writeCallback mWriteCb;
};

void AsyncClientImpl::registerConnectedCB(connectedCallback conn_cb)
{
    mConnCb = conn_cb;
}

void AsyncClientImpl::registerReadCB(readCallback read_cb)
{
    mReadCb = read_cb;
}

void AsyncClientImpl::registerWriteCB(writeCallback write_cb)
{
    mWriteCb = write_cb;
}

void AsyncClientImpl::connect()
{

}
