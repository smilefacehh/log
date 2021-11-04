#pragma once

#include <memory>
#include <mutex>

#include "Logger.h"

namespace Nlog
{
class LogMessage;

class StdoutLogger : public Logger
{
  public:
    StdoutLogger();
    ~StdoutLogger() override = default;

    void Write(const LogMessage& log_message) override;
    void Flush() override;

  private:
    // write_mutex_ protects the Write function
    std::mutex write_mutex_;
};

typedef std::shared_ptr<StdoutLogger> StdoutLoggerPtr;
}