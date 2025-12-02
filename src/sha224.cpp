#include "sha224.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cctype>

std::string SHA224::hash(const std::string& data) {
    unsigned char digest[SHA224_DIGEST_LENGTH];
    
    // Используем EVP API (рекомендуется в OpenSSL 3.0)
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return "";
    
    EVP_DigestInit_ex(ctx, EVP_sha224(), NULL);
    EVP_DigestUpdate(ctx, data.c_str(), data.length());
    EVP_DigestFinal_ex(ctx, digest, NULL);
    EVP_MD_CTX_free(ctx);
    
    return toHex(digest, SHA224_DIGEST_LENGTH);
}

std::string SHA224::hashWithSalt(const std::string& salt, const std::string& password) {
    std::string data = salt + password;
    return hash(data);
}

std::string SHA224::toHex(const unsigned char* data, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < length; i++) {
        ss << std::setw(2) << static_cast<unsigned>(data[i]);
    }
    
    return ss.str();
}

bool SHA224::isValidHex(const std::string& str) {
    if (str.empty()) return false;
    
    for (char c : str) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}