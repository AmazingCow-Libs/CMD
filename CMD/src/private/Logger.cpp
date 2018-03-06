// Header
#include "include/private/Logger.h"

CoreLog::Logger*
CMD::_private::get_logger_instance() noexcept
{
    acow_local_persist std::unique_ptr<CoreLog::Logger> sp_instance = nullptr;
    if(!sp_instance)
        sp_instance = acow::make_unique<CoreLog::Logger>();

    return sp_instance.get();
}