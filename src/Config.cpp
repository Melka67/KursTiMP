#include "Config.h"
#include <iostream>
#include <string>
#include <cstring>

Config Config::parseCommandLine(int argc, char* argv[]) {
    Config config;
    
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            printHelp();
            exit(0);
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            config.clientDbFile = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            config.logFile = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config.port = std::stoi(argv[++i]);
        }
    }
    
    return config;
}

void Config::printHelp() {
    std::cout << "Usage: server [options]\n"
              << "Options:\n"
              << "  -c <file>    Client database file (default: /etc/vcalc.conf)\n"
              << "  -l <file>    Log file (default: /var/log/vcalc.log)\n"
              << "  -p <port>    Port number (default: 33333)\n"
              << "  -h           Show this help message\n";
}