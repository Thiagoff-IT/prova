#ifndef CALENDARIO_H
#define CALENDARIO_H

#include <ctime>
#include <string>

struct Data {
    int y, m, d;
    Data(int yy=1970, int mm=1, int dd=1): y(yy), m(mm), d(dd) {}
};

// converte Data para time_t (UTC local)
time_t to_time_t(const Data &dt);

struct Calendario {
    static int diasDesde(const Data& dt);
    static bool dataPassou(const Data& dt);
};

struct MercadoFinanceiro {
    // valores "do dia"
    static double TR;
    static double DI;
    static double getTR();
    static double getDI();
};

#endif // CALENDARIO_H
