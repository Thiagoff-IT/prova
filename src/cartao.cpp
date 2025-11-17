#include "../include/cartao.h"
#include <cmath>

const HistoricoFaturas& Cartao::getHistoricoFaturas() const {
    return historico;
}

int Black::calcularPontuacao(double valorGasto) const {
    return static_cast<int>(std::floor(valorGasto / 2.0));
}

int Platinum::calcularPontuacao(double valorGasto) const {
    return static_cast<int>(std::floor(valorGasto / 4.0));
}
