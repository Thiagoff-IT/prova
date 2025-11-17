
#include "../include/calendario.h"

#include <ctime>

time_t to_time_t(const Data &dt) {
    std::tm t = {};
    t.tm_year = dt.y - 1900;
    t.tm_mon  = dt.m - 1;
    t.tm_mday = dt.d;
    t.tm_hour = 0;
    t.tm_min  = 0;
    t.tm_sec  = 0;
    t.tm_isdst = -1;
    return std::mktime(&t);
}

int Calendario::diasDesde(const Data& dt) {
    time_t then = to_time_t(dt);
    time_t now  = std::time(nullptr);
    double seconds = difftime(now, then);
    return static_cast<int>(seconds / (60*60*24));
}

bool Calendario::dataPassou(const Data& dt) {
    time_t then = to_time_t(dt);
    time_t now  = std::time(nullptr);
    return difftime(now, then) > 0;
}

double MercadoFinanceiro::TR = 0.002;
double MercadoFinanceiro::DI = 0.01;

double MercadoFinanceiro::getTR() { return TR; }
double MercadoFinanceiro::getDI() { return DI; }