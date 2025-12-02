#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
public:
    bool start(int port, const std::string& configFile);
    
private:
    void handleClient(int clientSocket);
};

#endif