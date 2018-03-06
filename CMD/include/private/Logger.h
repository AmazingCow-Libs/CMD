#pragma once

// AmazingCow Libs
#include "acow/cpp_goodies.h"
#include "CoreLog/CoreLog.h"
// CMD
#include "../CMD_Utils.h"

NS_CMD_BEGIN
namespace _private  {

CoreLog::Logger* get_logger_instance() noexcept;

} // namespace _private
NS_CMD_END