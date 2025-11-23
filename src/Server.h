#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include "AuthManager.h"
#include "Logger.h"
#include "Config.h"

class Server {
private:
    int serverSocket;
    struct sockaddr_in serverAddr;
    AuthManager authManager;
    Logger logger;
    bool running;
    
    bool setupSocket(int port);
    void cleanup();

public:
    Server(const Config& config);
    ~Server();
    bool start();
    void stop();

};

#endif