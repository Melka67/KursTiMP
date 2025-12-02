#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
public:
    static bool processVectors(int clientSocket);
    
private:
    static double calculateProduct(const std::vector<double>& vector);
};

#endif