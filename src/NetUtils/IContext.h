#ifndef __NET_UTILS_ICONTEXT_H__
#define __NET_UTILS_ICONTEXT_H__

#include <memory>
#include <common/base/IBaseObject.h>
#include <NetUtils/common.h>

NU_BEGIN

class IContext : CBASE_NAMESPACE::IBaseObject
{
public:
    IContext()
        : CBASE_NAMESPACE::IBaseObject("IContext")
    {}

    IContext(const char *tag)
        : CBASE_NAMESPACE::IBaseObject(tag)
    {}

    virtual ~IContext()
    {}
};
typedef std::shared_ptr<IContext> IContextSPtr;
typedef std::weak_ptr<IContext> IContextWPtr;

NU_END

#endif
