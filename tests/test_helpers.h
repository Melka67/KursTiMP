#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "auth.h"
#include "sha224.h"
#include <string>

// Класс-обертка для тестирования приватных методов
class TestAuth : public Auth {
public:
    static bool testValidateFormat(const std::string& login, 
                                  const std::string& salt, 
                                  const std::string& hash) {
        return validateFormat(login, salt, hash);
    }
};

#endif