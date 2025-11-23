#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    std::string clientDbFile = "/etc/vcalc.conf";
    std::string logFile = "/var/log/vcalc.log";
    int port = 33333;
    
    static Config parseCommandLine(int argc, char* argv[]);
    static void printHelp();
};

#endif