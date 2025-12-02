#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
public:
    static Logger& getInstance();
    
    bool init(const std::string& filename);
    void log(const std::string& message, bool isCritical = false);
    void close();
    
private:
    Logger() = default;
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::ofstream logFile;
};

#endif