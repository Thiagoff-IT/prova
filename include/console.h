#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include "calendario.h"

struct Console {
    static bool useColor();
    static std::string money(double v);
    static std::string date(const Data &d);
    static void header(const std::string &title);
    static void section(const std::string &title);
    static void kv(const std::string &k, const std::string &v);
};

#endif // CONSOLE_H
