#include "logger.hpp"
#include "paths.h"
#include "includes.hpp"
#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>


void AstralLogger::init() {
    if (!debugLog) return;
    
    std::filesystem::create_directories(LogsDir());
    logFile.open((LogsDir() / getLogFileName()).string());
    if (logFile.is_open()) {
        logFile << "================================\n";
        logFile << "Log started: " << getTimestamp() << "\n";
        logFile << "================================\n\n";
        logFile.flush();
    }
}

void AstralLogger::log(const std::string& message) {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "[" << getTimestamp() << "] " << message << "\n";
    logFile.flush();
}

void AstralLogger::logWarning(const std::string& message) {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "[" << getTimestamp() << "] [WARNING] " << message << "\n";
    logFile.flush();
}



void AstralLogger::logError(const std::string& message) {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "[" << getTimestamp() << "] [ERROR] " << message << "\n";
    logFile.flush();
}



void AstralLogger::logDebug(const std::string& message) {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "[" << getTimestamp() << "] [DEBUG] " << message << "\n";
    logFile.flush();
}



void AstralLogger::logSection(const std::string& sectionName) {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "\n--- " << sectionName << " ---\n";
    logFile.flush();
}

void AstralLogger::close() {
    if (!debugLog || !logFile.is_open()) return;
    logFile << "\n================================\n";
    logFile << "Log ended: " << getTimestamp() << "\n";
    logFile << "================================\n";
    logFile.close();
}

std::string AstralLogger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
}

std::string AstralLogger::getLogFileName() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "log_" << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S") << ".txt";
    return ss.str();
}