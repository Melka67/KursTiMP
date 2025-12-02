#include "server.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <filesystem>

Server server;

void signalHandler(int signum) {
    Logger::getInstance().log("Server stopped by signal", true);
    std::cout << "\nInterrupt signal received. Stopping server..." << std::endl;
    exit(signum);
}

void printHelp() {
    std::cout << "Usage: server [options]\n"
              << "Options:\n"
              << "  -h, --help            Show this help message\n"
              << "  -p PORT, --port PORT  Port to listen on (default: 33333)\n"
              << "  -c FILE, --config FILE Client database file (default: vcalc.conf)\n"
              << "  -l FILE, --log FILE   Log file (default: vcalc.log)\n";
}

int main(int argc, char* argv[]) {
    // Параметры по умолчанию - ЛОКАЛЬНЫЕ файлы
    int port = 33333;
    std::string configFile = "vcalc.conf";
    std::string logFile = "vcalc.log";
    
    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        }
        else if ((arg == "-p" || arg == "--port") && i + 1 < argc) {
            port = std::atoi(argv[++i]);
            if (port <= 0 || port > 65535) {
                std::cerr << "Invalid port number: " << port << std::endl;
                return 1;
            }
        }
        else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            configFile = argv[++i];
        }
        else if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
            logFile = argv[++i];
        }
        else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printHelp();
            return 1;
        }
    }
    
    // Инициализируем логгер
    if (!Logger::getInstance().init(logFile)) {
        std::cerr << "Cannot open log file: " << logFile << std::endl;
        return 1;
    }
    
    // Устанавливаем обработчик Ctrl+C
    signal(SIGINT, signalHandler);
    
    std::cout << "Starting server with parameters:\n"
              << "  Port: " << port << "\n"
              << "  Config file: " << configFile 
              << "\n  Log file: " << logFile << std::endl;
    
    Logger::getInstance().log("Server starting on port " + std::to_string(port));
    
    // Запускаем сервер
    if (!server.start(port, configFile)) {
        Logger::getInstance().log("Failed to start server", true);
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    return 0;
}