#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <streambuf>
#include <ostream>
#include <ios>

class Logger {
public:
    enum Level {
        Debug = 0,
        Info = 1,
        Warn = 2,
        Error = 3,
        Assertion = 4,
    };

    static void Init();

    Logger(Level type,std::string file, int line);
    ~Logger();

    void operator<< (const char* str);
    void operator<< (std::string str);
    void operator<< (bool val);
    void operator<< (short val);
    void operator<< (unsigned short val);
    void operator<< (int val);
    void operator<< (unsigned int val);
    void operator<< (long val);
    void operator<< (unsigned long val);
    void operator<< (long long val);
    void operator<< (unsigned long long val);
    void operator<< (float val);
    void operator<< (double val);
    void operator<< (long double val);
    void operator<< (void* val);
    void operator<< (std::streambuf* sb );
    void operator<< (std::ostream& (*pf)(std::ostream&));
    void operator<< (std::ios& (*pf)(std::ios&));
    void operator<< (std::ios_base& (*pf)(std::ios_base&));

private:
    bool m_terminate = false;
    std::ostringstream m_message; 
    Level m_level = Level::Debug;
};

#define     Log(level)    if(true) \
                                Logger(Logger::Level::level,__FILE__, __LINE__)

#define     Assert(assertion) if(!(assertion)) \
                                Logger(Logger::Level::Assertion,__FILE__, __LINE__)