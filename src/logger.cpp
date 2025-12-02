#include "logger.h"
#include <iostream>
#include <ctime>
#include <cstring>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::init(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Cannot open log file: " << filename << std::endl;
        return false;
    }
    return true;
}

void Logger::log(const std::string& message, bool isCritical) {
    if (!logFile.is_open()) {
        // Если файл не открыт, выводим в консоль
        std::cout << "[LOG] " << message << std::endl;
        return;
    }
    
    std::time_t now = std::time(nullptr);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    logFile << timeStr << " | ";
    logFile << (isCritical ? "CRITICAL" : "INFO") << " | ";
    logFile << message << std::endl;
    logFile.flush();
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger::~Logger() {
    close();
}