#ifndef __ASYNC_CLIENT_H__
#define __ASYNC_CLIENT_H__

class AsyncClientImpl;
class AsyncClient
{
public:
    typedef function<void(int)> connectedCallback;
    typedef function<void(int)> readCallback;
    typedef function<void(int)> writeCallback;

    AsyncClient(std::string ip, uint16_t port);

    ~AsyncClient();

    void registerConnectedCB(connectedCallback conn_cb);
    void registerReadCB(readCallback read_cb);
    void registerWriteCB(writeCallback write_cb);

    void connect();

private:
    std::shared_ptr<AsyncClientImpl> mImpl;
};

#endif
