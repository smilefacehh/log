## 特性
在robolog的基础上增加以下功能
1. 支持不同的logger自定义格式 ok
2. 支持不同的logger自定义等级（低优先级）
3. 支持console颜色区分头部、内容 ok
4. 支持插入外部实现，将日志打印到同一个文件或console里面 ok
5. 支持按大小切割文件（低优先级）
6. 支持每次运行重新写一个新的日志文件，不切割，主要用于调试 ok
7. 支持每次运行重新写一个新的日志文件，按文件大小切割

使用方法
```
auto console_logger = Logging::create<ConsoleLogger>();
console_logger->setLevel();
console_logger->setFormat();
Logging::addLogger(console_logger);

auto rotate_file_logger = Logging::create<RotateFileLogger>(filename);
rotate_file_logger->setLevel();
rotate_file_logger->setFormat();
Logging::addLogger(rotate_file_logger);

flush-time

DLOG << "hello" << ",world";
DLOG("hello,{0},{1}", "world", 1);

void debugLog(string s)
{
    if(plug_log) plug_log(s);
    
}

当设置了外部调用接管，需要将logMessage的回调指定为外部传入的LOG指针
```