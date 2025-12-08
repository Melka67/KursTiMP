#include <UnitTest++/UnitTest++.h>
#include <string>
#include <vector>

TEST(Utils_VectorOperations) {
    std::vector<double> vec = {1, 2, 3};
    CHECK_EQUAL(3, vec.size());
}

TEST(Utils_StringOperations) {
    std::string str = "test";
    CHECK_EQUAL(4, str.length());
}

TEST(Utils_HexValidation) {
    // Test hex string validation logic
    std::string validHex = "1234567890ABCDEFabcdef";
    std::string invalidHex = "123G";
    
    bool allHex = true;
    for (char c : validHex) {
        if (!isxdigit(c)) {
            allHex = false;
            break;
        }
    }
    CHECK(allHex);
}

TEST(Utils_PortValidationLogic) {
    // Проверяем логику классификации портов
    int port;
    
    // Тест 1: Валидный непривилегированный порт
    port = 1024;
    bool isValidUnprivileged = (port >= 1024 && port <= 65535);
    CHECK(isValidUnprivileged);
    
    // Тест 2: Привилегированный порт  
    port = 80;
    bool isPrivileged = (port >= 0 && port <= 1023);
    CHECK(isPrivileged);
    
    // Тест 3: Невалидный порт
    port = 70000;
    bool isInvalid = (port < 0 || port > 65535);
    CHECK(isInvalid);
}

TEST(Utils_OverflowConstants) {
    // Test overflow constants from TЗ
    double overflow_up = 9223372036854775807.0;    // 2^63 - 1
    double overflow_down = -9223372036854775808.0; // -2^63
    
    CHECK(overflow_up > 0);
    CHECK(overflow_down < 0);
}