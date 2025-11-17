#ifndef CARTAO_H
#define CARTAO_H

#include <memory>
#include "compra.h"

struct Cartao {
    HistoricoFaturas historico;
    double limiteCredito = 0.0;
    double valorDevido = 0.0;
    virtual int calcularPontuacao(double valorGasto) const = 0;
    virtual ~Cartao() = default;
    const HistoricoFaturas& getHistoricoFaturas() const;
};

struct Black : public Cartao {
    virtual int calcularPontuacao(double valorGasto) const override;
};

struct Platinum : public Cartao {
    virtual int calcularPontuacao(double valorGasto) const override;
};

#endif // CARTAO_H
