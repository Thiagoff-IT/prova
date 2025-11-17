#include "../include/extrato.h"
#include <iostream>

Movimentacao::Movimentacao(const Data& d, double v) : data(d), valor(v) {}

void Extrato::adicionarMov(const std::shared_ptr<Movimentacao>& m) {
    movs.push_back(m);
}

const std::vector<std::shared_ptr<Movimentacao>>& Extrato::getMovs() const {
    return movs;
}

void Extrato::imprimir() const {
    for (auto &m : movs) {
        std::cout << "  " << m->valor << " em " << m->data.y << "-" << m->data.m << "-" << m->data.d << "\n";
    }
}
