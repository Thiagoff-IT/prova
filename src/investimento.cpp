#include "../include/investimento.h"
#include "../include/calendario.h"

void Investimento::atualizarSaldo() {
    /* genérico */
}

void CDB::atualizarSaldo() {
    if (Calendario::dataPassou(this->dataVencimento)) {
        return;
    }

    double DI = MercadoFinanceiro::getDI();
    double rendimento = this->saldoAtual * (this->taxaDiariaPercentualDI * DI);
    this->saldoAtual += rendimento;

    int diasAplicado = Calendario::diasDesde(this->dataInicio);
    double aliquotaImposto = 0.0;
    if (diasAplicado <= 180) aliquotaImposto = 0.225;
    else if (diasAplicado <= 360) aliquotaImposto = 0.20;
    else if (diasAplicado <= 720) aliquotaImposto = 0.175;
    else aliquotaImposto = 0.15;

    double imposto = rendimento * aliquotaImposto;
    this->saldoLiquido = this->saldoAtual - imposto;
}

void LCI::atualizarSaldo() {
    if (Calendario::dataPassou(this->dataVencimento)) return;
    double DI = MercadoFinanceiro::getDI();
    double rendimento = this->saldoAtual * (this->taxaDiariaPercentualDI * DI);
    this->saldoAtual += rendimento;
}
