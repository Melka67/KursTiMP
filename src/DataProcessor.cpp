#include "DataProcessor.h"
#include <limits>
#include <cmath>

double DataProcessor::computeVectorProduct(const std::vector<double>& vector) {
    if (vector.empty()) {
        return 0.0;
    }
    
    double product = 1.0;
    
    for (double value : vector) {
        // Check for overflow
        if (product > 1.0 && value > std::numeric_limits<double>::max() / product) {
            return std::pow(2, 63) - 1; // Overflow up
        }
        if (product < -1.0 && value < std::numeric_limits<double>::lowest() / product) {
            return std::pow(2, 63); // Overflow down (using 2^63 as specified)
        }
        if (std::abs(product) < 1.0 && std::abs(value) > std::numeric_limits<double>::max() * std::abs(product)) {
            if ((product > 0 && value > 0) || (product < 0 && value < 0)) {
                return std::pow(2, 63) - 1; // Overflow up
            } else {
                return std::pow(2, 63); // Overflow down
            }
        }
        
        product *= value;
    }
    
    return product;
}

std::vector<double> DataProcessor::processVectors(const std::vector<std::vector<double>>& vectors) {
    std::vector<double> results;
    results.reserve(vectors.size());
    
    for (const auto& vector : vectors) {
        results.push_back(computeVectorProduct(vector));
    }
    
    return results;
}