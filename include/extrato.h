#ifndef EXTRATO_H
#define EXTRATO_H

#include <vector>
#include <memory>
#include "calendario.h"

struct Movimentacao {
    Data data;
    double valor;
    Movimentacao(const Data& d, double v);
};

struct Extrato {
    std::vector<std::shared_ptr<Movimentacao>> movs;
    void adicionarMov(const std::shared_ptr<Movimentacao>& m);
    const std::vector<std::shared_ptr<Movimentacao>>& getMovs() const;
    void imprimir() const;
};

#endif // EXTRATO_H
