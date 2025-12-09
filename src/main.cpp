#include "server.h"
#include "logger.h"
#include "args_parser.h"
#include <iostream>
#include <csignal>

Server server;

void signalHandler(int signum) {
    Logger::getInstance().log("Server stopped by signal", true);
    std::cout << "\nInterrupt signal received. Stopping server..." << std::endl;
    exit(signum);
}

int main(int argc, char* argv[]) {
    try {
        ServerConfig config = ArgsParser::parse(argc, argv);
        
        if (config.showHelp) {
            ArgsParser::printHelp();
            return 0;
        }
        
        // Инициализируем логгер
        if (!Logger::getInstance().init(config.logFile)) {
            std::cerr << "Cannot open log file: " << config.logFile << std::endl;
            return 1;
        }
        
        signal(SIGINT, signalHandler);
        
        std::cout << "Starting server with parameters:\n"
                  << "  Port: " << config.port << "\n"
                  << "  Config file: " << config.configFile 
                  << "\n  Log file: " << config.logFile << std::endl;
        
        Logger::getInstance().log("Server starting on port " + std::to_string(config.port));
        
        if (!server.start(config.port, config.configFile)) {
            Logger::getInstance().log("Failed to start server", true);
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ArgsParser::printHelp();
        return 1;
    }
    
    return 0;
}