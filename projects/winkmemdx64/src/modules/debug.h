#pragma once

class Debug {

public:
    static void info(const char * format, ...);
    static void error(const char * format, ...);
    static void warn(const char * format, ...);
};
