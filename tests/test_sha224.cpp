#include <UnitTest++/UnitTest++.h>
#include "../src/sha224.h"
#include <string>

TEST(SHA224_TestHashNotEmpty) {
    std::string result = SHA224::hash("test");
    CHECK(!result.empty());
}

TEST(SHA224_TestHashLength) {
    std::string result = SHA224::hash("test");
    // SHA-224 produces 56 hex characters (224 bits / 4 bits per hex char)
    CHECK_EQUAL(56, result.length());
}

TEST(SHA224_TestHashConsistency) {
    std::string result1 = SHA224::hash("same");
    std::string result2 = SHA224::hash("same");
    CHECK_EQUAL(result1, result2);
}

TEST(SHA224_TestHashDifferentForDifferentInput) {
    std::string result1 = SHA224::hash("input1");
    std::string result2 = SHA224::hash("input2");
    CHECK(result1 != result2);
}

TEST(SHA224_TestHashWithSalt) {
    std::string salt = "1234567890ABCDEF";
    std::string password = "password";
    std::string result = SHA224::hashWithSalt(salt, password);
    CHECK(!result.empty());
    CHECK_EQUAL(56, result.length());
}

TEST(SHA224_TestHashWithSaltConsistency) {
    std::string salt = "1234567890ABCDEF";
    std::string password = "password";
    std::string result1 = SHA224::hashWithSalt(salt, password);
    std::string result2 = SHA224::hashWithSalt(salt, password);
    CHECK_EQUAL(result1, result2);
}

TEST(SHA224_TestHashWithSaltDifferentSalt) {
    std::string salt1 = "1234567890ABCDEF";
    std::string salt2 = "FEDCBA0987654321";
    std::string password = "password";
    std::string result1 = SHA224::hashWithSalt(salt1, password);
    std::string result2 = SHA224::hashWithSalt(salt2, password);
    CHECK(result1 != result2);
}

TEST(SHA224_TestHashWithSaltDifferentPassword) {
    std::string salt = "1234567890ABCDEF";
    std::string password1 = "password1";
    std::string password2 = "password2";
    std::string result1 = SHA224::hashWithSalt(salt, password1);
    std::string result2 = SHA224::hashWithSalt(salt, password2);
    CHECK(result1 != result2);
}

TEST(SHA224_TestIsValidHexValid) {
    CHECK(SHA224::isValidHex("1234567890ABCDEFabcdef"));
}

TEST(SHA224_TestIsValidHexInvalid) {
    CHECK(!SHA224::isValidHex("123G")); // G is not hex
    CHECK(!SHA224::isValidHex(""));
}

TEST(SHA224_TestIsValidHexCaseInsensitive) {
    CHECK(SHA224::isValidHex("abcdef"));
    CHECK(SHA224::isValidHex("ABCDEF"));
}

TEST(SHA224_TestEmptyStringHash) {
    std::string result = SHA224::hash("");
    CHECK(!result.empty());
    CHECK_EQUAL(56, result.length());
}