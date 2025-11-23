#include <iostream>
#include <csignal>
#include "Server.h"
#include "Config.h"

Server* serverPtr = nullptr;

void signalHandler(int signal) {
    if (serverPtr != nullptr) {
        std::cout << "Received signal " << signal << ", shutting down server..." << std::endl;
        serverPtr->stop();
    }
}

int main(int argc, char* argv[]) {
    // Handle help parameter or no parameters
    if (argc == 1) {
        Config::printHelp();
        return 0;
    }
    
    try {
        Config config = Config::parseCommandLine(argc, argv);
        
        Server server(config);
        serverPtr = &server;
        
        // Setup signal handlers for graceful shutdown
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        std::cout << "Starting server on port " << config.port << std::endl;
        std::cout << "Client database: " << config.clientDbFile << std::endl;
        std::cout << "Log file: " << config.logFile << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        
        if (!server.start()) {
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}