#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>

class DataProcessor {
public:
    static double computeVectorProduct(const std::vector<double>& vector);
    static std::vector<double> processVectors(const std::vector<std::vector<double>>& vectors);
};

#endif