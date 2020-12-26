#ifndef __NET_UTILS_SERVICES_IWORKER_H__
#define __NET_UTILS_SERVICES_IWORKER_H__

#include <memory>
#include <common/base/IBaseObject.h>
#include <NetUtils/services/common.h>
#include <NetUtils/IContext.h>

NU_SER_BEGIN

class IWorker : CBASE_NAMESPACE::IBaseObject
{
public:
    IWorker()
        : mContext(nullptr)
    {}

    IWorker(IContextSPtr context)
        : mContext(context)
    {}

    virtual bool doWork() = 0;

protected:
    IContextSPtr mContext;
};
typedef std::shared_ptr<IWorker> IWorkerSptr;
typedef std::weak_ptr<IWorker> IWorkerWptr;

NU_SER_END

#endif
