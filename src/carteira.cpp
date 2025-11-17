#include "../include/carteira.h"
#include "../include/conta.h"
#include "../include/investimento.h"

double Carteira::calcularVolumeGerenciado() {
    double total = 0.0;
    for (auto &c : contas) {
        total += c->saldo;
        for (auto &inv : c->investimentos) {
            if (auto cdb = std::dynamic_pointer_cast<CDB>(inv)) {
                total += cdb->saldoLiquido;
            } else {
                total += inv->saldoAtual;
            }
        }
    }
    return total;
}

void Carteira::adicionarConta(const std::shared_ptr<Conta>& c) {
    contas.push_back(c);
}
