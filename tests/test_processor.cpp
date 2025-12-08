#include <UnitTest++/UnitTest++.h>
#include "../src/processor.h"
#include <vector>
#include <cmath>

TEST(Processor_CalculateProductEmpty) {
    std::vector<double> empty;
    double result = Processor::calculateProduct(empty);
    CHECK_EQUAL(0.0, result);
}

TEST(Processor_CalculateProductSingle) {
    std::vector<double> vec = {5.0};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(5.0, result);
}

TEST(Processor_CalculateProductPositive) {
    std::vector<double> vec = {2.0, 3.0, 4.0};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(24.0, result);
}

TEST(Processor_CalculateProductNegative) {
    std::vector<double> vec = {-2.0, 3.0, -4.0};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(24.0, result); // -2 * 3 * -4 = 24
}

TEST(Processor_CalculateProductWithZero) {
    std::vector<double> vec = {2.0, 0.0, 4.0};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(0.0, result);
}

TEST(Processor_CalculateProductLargeNumbers) {
    std::vector<double> vec = {1000.0, 1000.0, 1000.0};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(1000000000.0, result);
}

TEST(Processor_CalculateProductFractions) {
    std::vector<double> vec = {0.5, 0.5, 0.5};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(0.125, result);
}

TEST(Processor_CalculateProductMixed) {
    std::vector<double> vec = {2.5, -1.0, 4.0, 0.5};
    double result = Processor::calculateProduct(vec);
    CHECK_EQUAL(-5.0, result); // 2.5 * -1 * 4 * 0.5 = -5
}

TEST(Processor_CalculateProductPrecision) {
    std::vector<double> vec = {1.1, 1.1, 1.1};
    double result = Processor::calculateProduct(vec);
    double expected = 1.1 * 1.1 * 1.1;
    CHECK_CLOSE(expected, result, 0.0001);
}

TEST(Processor_CalculateProductOverflowDetection) {
    // Test with very large numbers that might overflow
    std::vector<double> vec = {1e100, 1e100, 1e100};
    double result = Processor::calculateProduct(vec);
    // Should handle overflow gracefully
    CHECK(!std::isnan(result));
}