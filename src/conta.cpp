#include "../include/conta.h"
#include "../include/calendario.h"
#include <cmath>

Conta::Conta(std::string n) : numero(n) {}

int Conta::calcularPontos() {
    int pontos = 0;
    for (auto &cart : cartoes) {
        const auto& hf = cart->getHistoricoFaturas();
        for (auto &f : hf.getFaturas()) {
            for (auto &c : f->getCompras()) {
                pontos += cart->calcularPontuacao(c->valor);
            }
        }
    }
    for (auto &m : extratoPontos.getMovs()) {
        pontos += static_cast<int>(m->valor);
    }
    return pontos;
}

ContaCorrente::ContaCorrente(std::string n) : Conta(n) {}

double ContaCorrente::calcularRendimentos(int dias) {
    return 0.0;
}

ContaPoupanca::ContaPoupanca(std::string n) : Conta(n) {}

double ContaPoupanca::calcularRendimentos(int dias) {
    int meses = dias / 30;
    double tr = MercadoFinanceiro::getTR();
    double taxaMensal = 0.005 + tr;
    return this->saldo * (std::pow(1.0 + taxaMensal, meses) - 1.0);
}
