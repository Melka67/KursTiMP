#include <UnitTest++/UnitTest++.h>
#include "../src/args_parser.h"
#include <vector>
#include <string>
#include <cstring>

TEST(ArgsParser_DefaultValues) {
    // Тест 1: Без аргументов - значения по умолчанию
    const char* argv[] = {"server"};
    int argc = 1;
    
    ServerConfig config = ArgsParser::parse(argc, (char**)argv);
    
    CHECK_EQUAL(33333, config.port);
    CHECK_EQUAL("vcalc.conf", config.configFile);
    CHECK_EQUAL("vcalc.log", config.logFile);
    CHECK(!config.showHelp);
}

TEST(ArgsParser_CustomPort) {
    // Тест 2: Пользовательский порт
    const char* argv[] = {"server", "-p", "8080"};
    int argc = 3;
    
    ServerConfig config = ArgsParser::parse(argc, (char**)argv);
    
    CHECK_EQUAL(8080, config.port);
    CHECK_EQUAL("vcalc.conf", config.configFile);
    CHECK_EQUAL("vcalc.log", config.logFile);
}

TEST(ArgsParser_CustomConfigAndLog) {
    // Тест 3: Пользовательские файлы
    const char* argv[] = {"server", "-c", "myconfig.conf", "-l", "mylog.log"};
    int argc = 5;
    
    ServerConfig config = ArgsParser::parse(argc, (char**)argv);
    
    CHECK_EQUAL(33333, config.port);
    CHECK_EQUAL("myconfig.conf", config.configFile);
    CHECK_EQUAL("mylog.log", config.logFile);
}

TEST(ArgsParser_HelpShort) {
    // Тест 4: Краткая справка
    const char* argv[] = {"server", "-h"};
    int argc = 2;
    
    ServerConfig config = ArgsParser::parse(argc, (char**)argv);
    
    CHECK(config.showHelp);
}

TEST(ArgsParser_HelpLong) {
    // Тест 5: Полная справка
    const char* argv[] = {"server", "--help"};
    int argc = 2;
    
    ServerConfig config = ArgsParser::parse(argc, (char**)argv);
    
    CHECK(config.showHelp);
}

TEST(ArgsParser_InvalidPortNegative) {
    // Тест 6: Отрицательный порт
    const char* argv[] = {"server", "-p", "-1"};
    int argc = 3;

    CHECK_THROW(ArgsParser::parse(argc, (char**)argv), std::invalid_argument);
}

TEST(ArgsParser_InvalidPortTooLarge) {
    // Тест 7: Порт больше 65535
    const char* argv[] = {"server", "-p", "70000"};
    int argc = 3;
    
    CHECK_THROW(ArgsParser::parse(argc, (char**)argv), std::invalid_argument);
}

TEST(ArgsParser_PrivilegedPort443) {
    // Тест 8: Недопустимый порт 443
    const char* argv[] = {"server", "-p", "443"};
    int argc = 3;
    
    CHECK_THROW(ArgsParser::parse(argc, (char**)argv), std::invalid_argument);
}

TEST(ArgsParser_UnknownOption) {
    // Тест 9: Неизвестная опция
    const char* argv[] = {"server", "-x", "value"};
    int argc = 3;
    
    CHECK_THROW(ArgsParser::parse(argc, (char**)argv), std::invalid_argument);
}

TEST(ArgsParser_MissingPortValue) {
    // Тест 10: Опция -p без значения
    const char* argv[] = {"server", "-p"};
    int argc = 2;
    
    CHECK_THROW(ArgsParser::parse(argc, (char**)argv), std::invalid_argument);
}