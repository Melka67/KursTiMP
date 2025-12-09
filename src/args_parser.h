#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <string>

struct ServerConfig {
    int port;
    std::string configFile;
    std::string logFile;
    bool showHelp;
};

class ArgsParser {
public:
    static ServerConfig parse(int argc, char* argv[]);
    static void printHelp();
    static bool validatePort(int port);
};

#endif