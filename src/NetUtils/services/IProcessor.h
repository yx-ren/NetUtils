#ifndef __NET_UTILS_SERVICES_IPROCESSOR_H__
#define __NET_UTILS_SERVICES_IPROCESSOR_H__

#include <memory>
#include <common/base/IBaseObject.h>
#include <NetUtils/services/common.h>
#include <NetUtils/IContext.h>

NU_SER_BEGIN

class IProcessor : public CBASE_NAMESPACE::IBaseObject
{
public:
    virtual ~IProcessor() {}

    virtual bool process(IContextSPtr context) = 0;
};
typedef std::shared_ptr<IProcessor> IProcessorSPtr;
typedef std::weak_ptr<IProcessor> IProcessorWPtr;

NU_SER_END

#endif
