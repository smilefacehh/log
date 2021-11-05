#include "src/Nlog.h"
#include "src/loggers/RotateFileLogger.h"
#include "src/loggers/StdoutLogger.h"

int main()
{
    // todo 定时刷日志。创建logger改成工厂模式，名字暴露出来给用户填写
    Nlog::Logging::SetLogSeverity(Nlog::LogSeverity::VERBOSE);
    auto stdout_logger = std::make_shared<Nlog::StdoutLogger>();
    stdout_logger->SetHeaderPattern("[%Y-%M-%D %h:%m:%s.%i][%T][%v][%F:%L][%U]");
    auto rotate_file_logger = Nlog::RotateFileLogger::Create("./log");
    Nlog::Logging::AddLogger(stdout_logger);
    Nlog::Logging::AddLogger(rotate_file_logger);
    Nlog::Logging::FlushEvery(std::chrono::seconds(3));

    {
        LOG_VERBOSE(-- -) << "格式与颜色效果展示:";
        LOG_VERBOSE(verbose) << "verbose log";
        LOG_DEBUG(debug) << "debug log";
        LOG_INFO(info) << "info log";
        LOG_WARN(warn) << "warn log";
        LOG_ERROR(error) << "error log";
        LOG_FATAL(fatal) << "fatal log";
    }
    {
        LOG_VERBOSE(-- -) << "基础数据类型:";
        int a = 1024;
        bool b = false;
        float c = 12.123456789;
        double d = 123456789.123456789;
        char e[20] = "test str";
        LOG_DEBUG(int) << a;
        LOG_DEBUG(bool) << b;
        LOG_DEBUG(float) << c;
        LOG_DEBUG(double) << d;
        LOG_DEBUG(char*) << e;
    }
    {
        LOG_VERBOSE(-- -) << "容器类型:";
        std::vector<std::string> a{"str1", "str2"};
        LOG_DEBUG(vector) << a;
        std::vector<std::vector<int>> b{{1, 2}, {3, 4, 5}, {4, 5}, {6, 7}};
        LOG_DEBUG(vector<vector>) << b;
        std::list<int> c{1, 2, 3};
        LOG_DEBUG(list) << c;
        std::deque<int> d{1, 2, 3};
        LOG_DEBUG(deque) << d;
        std::set<int> e{1, 2, 3};
        LOG_DEBUG(set) << e;
        std::map<int, std::string> f;
        f.insert(std::pair<int, std::string>(1, "str1"));
        f.insert(std::pair<int, std::string>(2, "str2"));
        LOG_DEBUG(map) << f;
        std::multiset<int> g{1, 1, 2, 3};
        LOG_DEBUG(multiset) << g;
        std::multimap<int, std::string> h;
        h.insert(std::pair<int, std::string>(1, "str1"));
        h.insert(std::pair<int, std::string>(1, "str2"));
        h.insert(std::pair<int, std::string>(2, "str3"));
        LOG_DEBUG(multimap) << h;
        std::queue<int> i;
        i.push(1);
        i.push(2);
        i.push(3);
        i.push(4);
        LOG_DEBUG(queue) << i;
        std::stack<int> j;
        j.push(1);
        j.push(2);
        j.push(3);
        j.push(4);
        LOG_DEBUG(stack) << j;
    }
    {
        LOG_DEBUG(#) << 1.123456789;
        LOG_DEBUG(#) << 1e7;
        LOG_DEBUG(#) << 0x12;
        LOG_DEBUG(#) << 012;
    }

    return 0;
}