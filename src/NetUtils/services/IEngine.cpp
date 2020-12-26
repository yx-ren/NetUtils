#include <NetUtils/services/IEngine.h>
#include <common/base/logger/LoggerManager.h>

using namespace CBASE_LOGGER_NAMESPACE;

NU_SER_BEGIN

bool IEngine::restart()
{
    stop();

    if (!start())
    {
        CBT_WARN("IEngine", "restart() start engine:" << toString() << " failed");
        return false;
    }

    return true;
}

NU_SER_END
