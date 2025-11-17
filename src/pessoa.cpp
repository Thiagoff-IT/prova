#include "../include/pessoa.h"
#include "../include/conta.h"
#include "../include/console.h"
#include <unordered_map>
#include <algorithm>

Pessoa::Pessoa(std::string n, std::string c) : nome(n), cpf(c) {}

Gerente::Gerente(std::string n, std::string c) : Pessoa(n, c) {}

Cliente::Cliente(std::string n, std::string cpf_) : Pessoa(n, cpf_) {}

void Cliente::adicionarConta(const std::shared_ptr<Conta>& c) {
    contas.push_back(c);
}

void Cliente::imprimirListaDeEstabelecimentosFavoritos() {
    std::unordered_map<std::string, double> gastos;
    for (auto &conta : contas) {
        for (auto &cart : conta->cartoes) {
            const auto& hf = cart->getHistoricoFaturas();
            for (auto &f : hf.getFaturas()) {
                for (auto &cp : f->getCompras()) {
                    gastos[cp->estabelecimento] += cp->valor;
                }
            }
        }
    }

    std::vector<std::pair<std::string, double>> lista;
    lista.reserve(gastos.size());
    for (auto &p : gastos) lista.emplace_back(p.first, p.second);

    std::sort(lista.begin(), lista.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    Console::section("Estabelecimentos favoritos (por valor gasto)");
    for (auto &it : lista) {
        Console::kv(it.first, Console::money(it.second));
    }
}

ClienteShared::ClienteShared(std::string n, std::string cpf_) : Cliente(n, cpf_) {}

void ClienteShared::adicionarContaSafe(const std::shared_ptr<Conta>& c) {
    contas.push_back(c);
    c->correntistas.push_back(std::static_pointer_cast<Cliente>(shared_from_this()));
}
