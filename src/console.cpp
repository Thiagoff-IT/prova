#include "../include/console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>

bool Console::useColor() {
    return isatty(fileno(stdout));
}

std::string Console::money(double v) {
    std::ostringstream oss;
    oss << "R$ " << std::fixed << std::setprecision(2) << v;
    return oss.str();
}

std::string Console::date(const Data &d) {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << d.y << "-"
        << std::setw(2) << std::setfill('0') << d.m << "-"
        << std::setw(2) << std::setfill('0') << d.d;
    return oss.str();
}

void Console::header(const std::string &title) {
    const std::string sep(60, '=');
    if (useColor()) std::cout << "\033[1;34m";
    std::cout << sep << "\n";
    std::cout << "  " << title << "\n";
    std::cout << sep << "\n";
    if (useColor()) std::cout << "\033[0m";
}

void Console::section(const std::string &title) {
    const std::string sep(60, '-');
    if (useColor()) std::cout << "\033[1;32m";
    std::cout << title << "\n";
    std::cout << sep << "\n";
    if (useColor()) std::cout << "\033[0m";
}

void Console::kv(const std::string &k, const std::string &v) {
    std::cout << std::left << std::setw(30) << k << " : " << v << "\n";
}
