/**
 * @file args_parser.cpp
 * @brief Реализация парсера аргументов командной строки
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "args_parser.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
 * @brief Разбирает аргументы командной строки
 * 
 * @details Проходит по всем аргументам, распознает опции и их значения.
 * Устанавливает значения по умолчанию если опции не указаны.
 * 
 * @param argc Количество аргументов
 * @param argv Массив аргументов
 * @return ServerConfig Конфигурация сервера
 * @throw std::invalid_argument при неизвестной опции или неверном порте
 * 
 * @see ServerConfig
 * @see validatePort
 */
ServerConfig ArgsParser::parse(int argc, char* argv[]) {
    ServerConfig config;
    
    // Значения по умолчанию
    config.port = 33333;
    config.configFile = "vcalc.conf";
    config.logFile = "vcalc.log";
    config.showHelp = false;
    
    // Парсим аргументы
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            config.showHelp = true;
            return config;
        }
        else if ((arg == "-p" || arg == "--port") && i + 1 < argc) {
            config.port = std::atoi(argv[++i]);
            if (!validatePort(config.port)) {
                throw std::invalid_argument("Invalid port number: " + std::to_string(config.port));
            }
        }
        else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            config.configFile = argv[++i];
        }
        else if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
            config.logFile = argv[++i];
        }
        else {
            throw std::invalid_argument("Unknown option: " + arg);
        }
    }
    
    return config;
}

/**
 * @brief Проверяет корректность номера порта
 * 
 * @param port Номер порта для проверки
 * @return true если порт в допустимом диапазоне
 * @return false если порт зарезервирован или вне диапазона
 * 
 * @note Системные порты (0-1023) требуют прав администратора
 */
bool ArgsParser::validatePort(int port) {
    if (port < 0 || port > 65535) {
        return false;
    }
    
    // Защита от привилегированных портов (0-1023)
    if (port >= 0 && port <= 1023) {
        return false;
    }
    
    return true;
}

/**
 * @brief Выводит справку по использованию программы
 * 
 * Форматированный вывод доступных опций и их описания.
 * Вызывается при указании -h или --help, или при ошибке парсинга.
 */
void ArgsParser::printHelp() {
    std::cout << "Usage: server [options]\n"
              << "Options:\n"
              << "  -h, --help            Show this help message\n"
              << "  -p PORT, --port PORT  Port to listen on (default: 33333)\n"
              << "                         PORT must be in range 1024-65535\n"
              << "  -c FILE, --config FILE Client database file (default: vcalc.conf)\n"
              << "  -l FILE, --log FILE   Log file (default: vcalc.log)\n";
}