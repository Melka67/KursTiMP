#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
private:
    std::ofstream logFile;
    std::mutex logMutex;
    std::string filename;

    std::string getCurrentTime();
    std::string levelToString(LogLevel level);

public:
    Logger(const std::string& filename = "/var/log/vcalc.log");
    ~Logger();
    
    void log(LogLevel level, const std::string& message, const std::string& params = "");
    void setLogFile(const std::string& filename);
};

#endif