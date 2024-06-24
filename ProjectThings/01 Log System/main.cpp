#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> // basic file logging
#include <spdlog/sinks/rotating_file_sink.h> // support for rotating file logging

// 日志系统
// 使用open source header only library spdlog, "include what you need"
// Ref: https://github.com/gabime/spdlog/wiki



int main()
{
    // 1. info
    spdlog::info("Hello {}!", "World");

    // 2. file logging
    {
        try {
            auto logger = spdlog::basic_logger_mt("basic_logger", "D:/logs/basic.txt");
            logger->info("Hello {}! This is my file log.", "Aspect");

            // 轮替日志器
            // create a file rotating logger with 5mb size max and 3 rotated files
            auto file_logger = spdlog::rotating_logger_mt("file_logger", "D:/logs/myfilename", 1024 * 1024 * 5, 3);
            file_logger->info("this is rotating file logger!");
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cout << "Log Initialization failed: " << ex.what() << std::endl;
        }
    }
    
    return 0;
}