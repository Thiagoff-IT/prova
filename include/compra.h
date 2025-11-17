#ifndef COMPRA_H
#define COMPRA_H

#include <string>
#include <vector>
#include <memory>
#include "calendario.h"

struct Compra {
    std::string estabelecimento;
    Data data;
    double valor;
    Compra(const std::string& e, const Data& d, double v);
};

struct Fatura {
    std::vector<std::shared_ptr<Compra>> compras;
    void adicionarCompra(const std::shared_ptr<Compra>& c);
    double calcularTotal() const;
    const std::vector<std::shared_ptr<Compra>>& getCompras() const;
    void imprimir() const;
};

struct HistoricoFaturas {
    std::vector<std::shared_ptr<Fatura>> faturas;
    void adicionarFatura(const std::shared_ptr<Fatura>& f);
    const std::vector<std::shared_ptr<Fatura>>& getFaturas() const;
};

#endif // COMPRA_H
