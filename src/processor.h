#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
public:
    static bool processVectors(int clientSocket);
    
// Делаем методы публичными для тестов
#ifdef UNIT_TESTS
public:
#else
private:
#endif
    static double calculateProduct(const std::vector<double>& vector);
};

#endif