#pragma once
#include <fstream>
#include <string>
#include <sstream>
// ai updated to allow AstralLogger::log("Recording macro: {}", macroName); format style logging
class AstralLogger {
private:
    static inline std::ofstream logFile;


public:
    static void init();
    
    static void log(const std::string& message);
    
    template<typename... Args>
    static void log(const std::string& format, Args&&... args) {
        std::string formatted = formatString(format, std::forward<Args>(args)...);
        log(formatted);
    }
    
    static void close();
    static void logWarning(const std::string& message);
    static void logError(const std::string& message);
    static void logDebug(const std::string& message);
    static void logSection(const std::string& sectionName);
    
    template<typename... Args>
    static void logWarning(const std::string& format, Args&&... args) {
        std::string formatted = formatString(format, std::forward<Args>(args)...);
        logWarning(formatted);
    }
    
    template<typename... Args>
    static void logError(const std::string& format, Args&&... args) {
        std::string formatted = formatString(format, std::forward<Args>(args)...);
        logError(formatted);
    }
    
    template<typename... Args>
    static void logDebug(const std::string& format, Args&&... args) {
        std::string formatted = formatString(format, std::forward<Args>(args)...);
        logDebug(formatted);
    }
    
private:
    static std::string getTimestamp();
    static std::string getLogFileName();
    
    template<typename T>
    static std::string toString(T&& value) {
        std::stringstream ss;
        ss << std::forward<T>(value);
        return ss.str();
    }
    
    template<typename... Args>
    static std::string formatString(const std::string& format, Args&&... args) {
        std::string result = format;
        std::vector<std::string> argStrings = {toString(std::forward<Args>(args))...};
        
        size_t argIndex = 0;
        size_t pos = 0;
        while ((pos = result.find("{}", pos)) != std::string::npos && argIndex < argStrings.size()) {
            result.replace(pos, 2, argStrings[argIndex++]);
            pos += argStrings[argIndex - 1].length();
        }
        
        return result;
    }
};