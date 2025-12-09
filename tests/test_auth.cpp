#include <UnitTest++/UnitTest++.h>
#include "../src/auth.h"
#include <string>

TEST(Auth_ValidateFormatCorrect) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEF"; // 16 hex chars
    std::string hash = std::string(56, 'A'); // 56 hex chars
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(result);
}

TEST(Auth_ValidateFormatWrongLogin) {
    std::string login = "wrong";
    std::string salt = "1234567890ABCDEF";
    std::string hash = std::string(56, 'A');
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatShortSalt) {
    std::string login = "user";
    std::string salt = "123"; // Too short
    std::string hash = std::string(56, 'A');
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatLongSalt) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEFG"; // 17 chars
    std::string hash = std::string(56, 'A');
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatShortHash) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEF";
    std::string hash = std::string(55, 'A'); // 55 chars
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatLongHash) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEF";
    std::string hash = std::string(57, 'A'); // 57 chars
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatInvalidHexSalt) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEG"; // G is not hex
    std::string hash = std::string(56, 'A');
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}

TEST(Auth_ValidateFormatInvalidHexHash) {
    std::string login = "user";
    std::string salt = "1234567890ABCDEF";
    std::string hash = std::string(55, 'A') + "G"; // G is not hex
    
    bool result = Auth::validateFormat(login, salt, hash);
    CHECK(!result);
}