#ifndef INVESTIMENTO_H
#define INVESTIMENTO_H

#include "calendario.h"

struct Investimento {
    double saldoAtual = 0.0;
    Data dataInicio;
    virtual ~Investimento() = default;
    virtual void atualizarSaldo();
};

struct CDB : public Investimento {
    double taxaDiariaPercentualDI = 0.0;
    double saldoLiquido = 0.0;
    Data dataVencimento;
    virtual void atualizarSaldo() override;
};

struct LCI : public Investimento {
    double taxaDiariaPercentualDI = 0.0;
    Data dataVencimento;
    virtual void atualizarSaldo() override;
};

#endif // INVESTIMENTO_H
