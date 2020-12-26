#ifndef __NET_UTILS_SERVICES_IENGINE_H__
#define __NET_UTILS_SERVICES_IENGINE_H__

#include <memory>
#include <common/base/IBaseService.h>
#include <NetUtils/IContext.h>
#include <NetUtils/services/common.h>

NU_SER_BEGIN

class IEngine : public CBASE_NAMESPACE::IBaseService
{
public:
    IEngine(const char *tag)
        : CBASE_NAMESPACE::IBaseService(tag)
        , mNextEngine(nullptr)
    {}

    virtual ~IEngine() {}

    virtual bool restart();

    virtual bool process(IContextSPtr context) = 0;

protected:
    std::shared_ptr<IEngine> mNextEngine;
};
typedef std::shared_ptr<IEngine> IEngineSptr;
typedef std::weak_ptr<IEngine> IEngineWptr;

NU_SER_END

#endif
