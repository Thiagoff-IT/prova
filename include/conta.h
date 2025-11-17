#ifndef CONTA_H
#define CONTA_H

#include <string>
#include <vector>
#include <memory>
#include "extrato.h"
#include "cartao.h"
#include "investimento.h"

struct Conta : public std::enable_shared_from_this<Conta> {
    std::string numero;
    double saldo = 0.0;
    Extrato extratoFinanceiro;
    Extrato extratoPontos;
    std::vector<std::shared_ptr<Investimento>> investimentos;
    std::vector<std::shared_ptr<Cartao>> cartoes;
    std::vector<std::shared_ptr<class Cliente>> correntistas;

    Conta(std::string n="");
    virtual ~Conta() = default;
    virtual double calcularRendimentos(int dias) = 0;
    virtual int calcularPontos();
};

struct ContaCorrente : public Conta {
    ContaCorrente(std::string n="");
    virtual double calcularRendimentos(int dias) override;
};

struct ContaPoupanca : public Conta {
    ContaPoupanca(std::string n="");
    virtual double calcularRendimentos(int dias) override;
};

#endif // CONTA_H
