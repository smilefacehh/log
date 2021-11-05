#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "LogSeverity.h"
#include "PeriodicWorker.h"
#include "Utils.h"

namespace Nlog {
class Logging
{
  public:
  private:
    LogSeverity log_severity_ = LogSeverity::DEBUG;
    std::unique_ptr<>
};
}  // namespace Nlog