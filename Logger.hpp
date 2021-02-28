#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

//Currently the logger is thread safe

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define LOF_FILE_NAME "results.log"
std::string GetDateAndTime();
std::string HexDump(std::vector<uint8_t>& data);
std::string DecDump(std::vector<uint8_t>& data);

class Logger {
public:
    static void WipeFile() {
        FILE* log_file;
        //Create an empty file for output operations. If a file with the same name already exists, its contents are discarded and the file is treated as a new empty file.
        log_file = fopen(LOF_FILE_NAME, "w");
        if(log_file == NULL)
        {
            std::string error_msg = "[ERROR] Could not open log file to write to.\n";
            fprintf(stdout, "%s", error_msg.c_str());
            fflush(stdout);  
        } else {
            fclose(log_file);
        }
    }

    Logger() {}
    ~Logger();

    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_ASSERT,
    };

    std::ostringstream& SetMessage(LogLevel type,std::string file, int line);

private:
    bool terminate = false;
    std::ostringstream message; 

    Logger(const Logger&);
    Logger& operator =(const Logger&);

    std::string LevelToString(LogLevel level);    
};

#ifdef DEBUG_BUILD
#define     LogDebug    if(true) \
                            Logger().SetMessage(Logger::LOG_DEBUG,__FILE__, __LINE__)
#else
#define     LogDebug    if(false) \
                            Logger().SetMessage(Logger::LOG_DEBUG,__FILE__, __LINE__)     //This will never be used and is only here to allow code to compiler without DEBUG defined.
#endif //DEBUG_BUILD

#define     LogInfo     if(true) \
                            Logger().SetMessage(Logger::LOG_INFO,__FILE__, __LINE__)

#define     LogWarn     if(true) \
                            Logger().SetMessage(Logger::LOG_WARN,__FILE__, __LINE__)

#define     LogError    if(true) \
                            Logger().SetMessage(Logger::LOG_ERROR,__FILE__, __LINE__)

#define     LogAssert(assertion) if(!assertion) \
                                    Logger().SetMessage(Logger::LOG_ASSERT,__FILE__, __LINE__)


#endif //LOGGER_HPP                        