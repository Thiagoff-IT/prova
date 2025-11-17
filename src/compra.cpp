#include "../include/compra.h"
#include "../include/console.h"
#include <sstream>

Compra::Compra(const std::string& e, const Data& d, double v)
    : estabelecimento(e), data(d), valor(v) {}

void Fatura::adicionarCompra(const std::shared_ptr<Compra>& c) {
    compras.push_back(c);
}

double Fatura::calcularTotal() const {
    double s = 0.0;
    for (auto &c : compras) s += c->valor;
    return s;
}

const std::vector<std::shared_ptr<Compra>>& Fatura::getCompras() const {
    return compras;
}

void Fatura::imprimir() const {
    std::ostringstream title;
    title << "Fatura (total " << Console::money(calcularTotal()) << ")";
    Console::section(title.str());
    for (auto &c : compras) {
        Console::kv(c->estabelecimento, Console::money(c->valor));
    }
}

void HistoricoFaturas::adicionarFatura(const std::shared_ptr<Fatura>& f) {
    faturas.push_back(f);
}

const std::vector<std::shared_ptr<Fatura>>& HistoricoFaturas::getFaturas() const {
    return faturas;
}
