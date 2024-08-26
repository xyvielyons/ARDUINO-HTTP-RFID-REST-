#ifndef STRINGPROCESSES_H
#define STRINGPROCESSES_H
#include <Arduino.h>
class StringProcesses{
    public:
        StringProcesses() = default;
        String removeFirstSpace(String str);
};
#endif