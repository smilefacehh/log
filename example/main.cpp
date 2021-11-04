#include <memory>
#include "../install/include/Nlog/Nlog.h"
#include "../install/include/Nlog/loggers/StdoutLogger.h"
#include "../install/include/Nlog/loggers/RotateFileLogger.h"

int main()
{
    Nlog::Logging::SetLogSeverity(Nlog::LogSeverity::DEBUG);
    auto stdout_logger = std::make_shared<Nlog::StdoutLogger>();
    auto rotate_file_logger = Nlog::RotateFileLogger::Create("./log");
    Nlog::Logging::AddLogger(stdout_logger);
    Nlog::Logging::AddLogger(rotate_file_logger);

    Nlog::LOG_DEBUG(@) << "hello debug";
    Nlog::LOG_INFO(@) << "hello info";
    Nlog::LOG_WARN(@) << "hello warn";
    Nlog::LOG_ERROR(@) << "hello error";
    Nlog::LOG_FATAL(@) << "hello fatal";

    return 0;
}