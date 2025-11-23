#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <iostream>

Logger::Logger(const std::string& filename) : filename(filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Cannot open log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message, const std::string& params) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (!logFile.is_open()) {
        std::cerr << "Log file not open!" << std::endl;
        return;
    }
    
    std::string criticality = (level == LogLevel::CRITICAL || level == LogLevel::ERROR) 
                            ? "critical" : "non-critical";
    
    logFile << "[" << getCurrentTime() << "] "
            << "[" << levelToString(level) << "] "
            << "[" << criticality << "] "
            << message;
    
    if (!params.empty()) {
        logFile << " [" << params << "]";
    }
    
    logFile << std::endl;
    logFile.flush();
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    this->filename = filename;
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Cannot open log file: " << filename << std::endl;
    }
}