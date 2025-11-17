#ifndef CARTEIRA_H
#define CARTEIRA_H

#include <vector>
#include <memory>

struct Conta;

struct Carteira {
    std::vector<std::shared_ptr<Conta>> contas;
    double calcularVolumeGerenciado();
    void adicionarConta(const std::shared_ptr<Conta>& c);
};

#endif // CARTEIRA_H
