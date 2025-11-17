#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <vector>
#include <memory>

struct Carteira;
struct Conta; // forward declarations

struct Pessoa {
    std::string nome;
    std::string cpf;
    Pessoa(std::string n="", std::string c="");
    virtual ~Pessoa() = default;
};

struct Gerente : public Pessoa {
    std::shared_ptr<Carteira> carteira;
    Gerente(std::string n="", std::string c="");
};

struct Cliente : public Pessoa {
    std::vector<std::shared_ptr<Conta>> contas;
    Cliente(std::string n="", std::string cpf_="");
    void adicionarConta(const std::shared_ptr<Conta>& c);
    void imprimirListaDeEstabelecimentosFavoritos();
};

struct ClienteShared : public Cliente, public std::enable_shared_from_this<ClienteShared> {
    ClienteShared(std::string n="", std::string cpf_="");
    void adicionarContaSafe(const std::shared_ptr<Conta>& c);
};

#endif // PESSOA_H
