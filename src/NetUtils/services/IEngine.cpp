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

void IEngine::setNextEngine(std::shared_ptr<IEngine> engine)
{
    mNextEngine = engine;
}

std::shared_ptr<IEngine> IEngine::getNextEngine() const
{
    return mNextEngine;
}

NU_SER_END
