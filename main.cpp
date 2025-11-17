// full_minimal_bank.cpp
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

// -----------------------------
// Util: Data simples
// -----------------------------
struct Data {
    int y, m, d;
    Data(int yy=1970, int mm=1, int dd=1): y(yy), m(mm), d(dd) {}
};

time_t to_time_t(const Data &dt) {
    std::tm t = {};
    t.tm_year = dt.y - 1900;
    t.tm_mon  = dt.m - 1;
    t.tm_mday = dt.d;
    t.tm_hour = 0;
    t.tm_min  = 0;
    t.tm_sec  = 0;
    t.tm_isdst = -1;
    return std::mktime(&t);
}

// -----------------------------
// Util: Interface de Console
// -----------------------------
struct Console {
    static bool useColor() {
        return isatty(fileno(stdout));
    }
    static string money(double v) {
        ostringstream oss;
        oss << "R$ " << fixed << setprecision(2) << v;
        return oss.str();
    }
    static string date(const Data &d) {
        ostringstream oss;
        oss << setw(4) << setfill('0') << d.y << "-"
            << setw(2) << setfill('0') << d.m << "-"
            << setw(2) << setfill('0') << d.d;
        return oss.str();
    }
    static void header(const string &title) {
        const string sep(60, '=');
        if (useColor()) cout << "\033[1;34m"; // bold blue
        cout << sep << "\n";
        cout << "  " << title << "\n";
        cout << sep << "\n";
        if (useColor()) cout << "\033[0m";
    }
    static void section(const string &title) {
        const string sep(60, '-');
        if (useColor()) cout << "\033[1;32m"; // bold green
        cout << title << "\n";
        cout << sep << "\n";
        if (useColor()) cout << "\033[0m";
    }
    static void kv(const string &k, const string &v) {
        cout << left << setw(30) << k << " : " << v << "\n";
    }
};

// -----------------------------
// Calendario e MercadoFinanceiro
// -----------------------------
struct Calendario {
    static int diasDesde(const Data& dt) {
        time_t then = to_time_t(dt);
        time_t now  = std::time(nullptr);
        double seconds = difftime(now, then);
        return static_cast<int>(seconds / (60*60*24));
    }
    static bool dataPassou(const Data& dt) {
        time_t then = to_time_t(dt);
        time_t now  = std::time(nullptr);
        return difftime(now, then) > 0;
    }
};

struct MercadoFinanceiro {
    // valores "do dia"
    static double TR;
    static double DI;
    static double getTR() { return TR; }
    static double getDI() { return DI; }
};
double MercadoFinanceiro::TR = 0.005; // exemplo
double MercadoFinanceiro::DI = 0.01;  // exemplo

// -----------------------------
// Movimentacao / Extrato
// -----------------------------
struct Movimentacao {
    Data data;
    double valor; // positivo = entrada, negativo = saída. For points extrato, valor is points (can be negative)
    Movimentacao(const Data& d, double v): data(d), valor(v) {}
};

struct Extrato {
    vector<shared_ptr<Movimentacao>> movs;
    void adicionarMov(const shared_ptr<Movimentacao>& m) { movs.push_back(m); }
    const vector<shared_ptr<Movimentacao>>& getMovs() const { return movs; }
    void imprimir() const {
        for (auto &m : movs) {
            cout << "  " << m->valor << " em " << m->data.y << "-" << m->data.m << "-" << m->data.d << "\n";
        }
    }
};

// -----------------------------
// Compra / Fatura / HistoricoFaturas
// -----------------------------
struct Compra {
    string estabelecimento;
    Data data;
    double valor;
    Compra(const string& e, const Data& d, double v): estabelecimento(e), data(d), valor(v) {}
};

struct Fatura {
    vector<shared_ptr<Compra>> compras;
    void adicionarCompra(const shared_ptr<Compra>& c) { compras.push_back(c); }
    double calcularTotal() const {
        double s = 0.0;
        for (auto &c : compras) s += c->valor;
        return s;
    }
    const vector<shared_ptr<Compra>>& getCompras() const { return compras; }
    void imprimir() const {
        ostringstream title;
        title << "Fatura (total " << Console::money(calcularTotal()) << ")";
        Console::section(title.str());
        for (auto &c : compras) {
            Console::kv(c->estabelecimento, Console::money(c->valor));
        }
    }
};

struct HistoricoFaturas {
    vector<shared_ptr<Fatura>> faturas;
    void adicionarFatura(const shared_ptr<Fatura>& f) { faturas.push_back(f); }
    const vector<shared_ptr<Fatura>>& getFaturas() const { return faturas; }
};

// -----------------------------
// Pessoa, Cliente, Gerente (mínimos)
// -----------------------------
struct Conta; // forward

struct Pessoa {
    string nome;
    string cpf;
    Pessoa(string n="", string c=""): nome(n), cpf(c) {}
    virtual ~Pessoa() = default;
};

struct Carteira {
    vector<shared_ptr<Conta>> contas;
    double calcularVolumeGerenciado(); // implementado depois
    void adicionarConta(const shared_ptr<Conta>& c) { contas.push_back(c); }
};

struct Gerente: public Pessoa {
    shared_ptr<Carteira> carteira;
    Gerente(string n="", string c=""): Pessoa(n,c) {}
};

// -----------------------------
// Investimento (base) e CDB
// -----------------------------
struct Investimento {
    double saldoAtual = 0.0;
    Data dataInicio;
    virtual ~Investimento() = default;
    virtual void atualizarSaldo() { /* genérico */ }
};

struct CDB : public Investimento {
    double taxaDiariaPercentualDI = 0.0; // ex: 0.5 (50% do DI) -> aqui usamos coeficiente direto
    double saldoLiquido = 0.0;
    Data dataVencimento;

    virtual void atualizarSaldo() override {
        // 1) Só atualiza se NÃO venceu
        if (Calendario::dataPassou(this->dataVencimento)) {
            return;
        }

        // 2) rendimento = saldoAtual * (taxaDiariaPercentualDI * DI)
        double DI = MercadoFinanceiro::getDI();
        double rendimento = this->saldoAtual * (this->taxaDiariaPercentualDI * DI);

        // atualizar saldo atual
        this->saldoAtual += rendimento;

        // 3) ajustar alíquota conforme tempo de aplicação
        int diasAplicado = Calendario::diasDesde(this->dataInicio);
        double aliquotaImposto = 0.0;
        if (diasAplicado <= 180) aliquotaImposto = 0.225;
        else if (diasAplicado <= 360) aliquotaImposto = 0.20;
        else if (diasAplicado <= 720) aliquotaImposto = 0.175;
        else aliquotaImposto = 0.15;

        // 4) saldoLiquido = saldoAtual - impostoSobreORendimento
        double imposto = rendimento * aliquotaImposto;
        this->saldoLiquido = this->saldoAtual - imposto;
    }
};

struct LCI : public Investimento {
    double taxaDiariaPercentualDI = 0.0;
    Data dataVencimento;
    virtual void atualizarSaldo() override {
        // simplificado: não tributa aqui (LCI normalmente isento)
        if (Calendario::dataPassou(this->dataVencimento)) return;
        double DI = MercadoFinanceiro::getDI();
        double rendimento = this->saldoAtual * (this->taxaDiariaPercentualDI * DI);
        this->saldoAtual += rendimento;
    }
};

// -----------------------------
// Cartões
// -----------------------------
struct Cartao {
    HistoricoFaturas historico;
    double limiteCredito = 0.0;
    double valorDevido = 0.0;
    virtual int calcularPontuacao(double valorGasto) const = 0;
    virtual ~Cartao() = default;
    const HistoricoFaturas& getHistoricoFaturas() const { return historico; }
};

struct Black : public Cartao {
    virtual int calcularPontuacao(double valorGasto) const override {
        return static_cast<int>(floor(valorGasto / 2.0));
    }
};

struct Platinum : public Cartao {
    virtual int calcularPontuacao(double valorGasto) const override {
        return static_cast<int>(floor(valorGasto / 4.0));
    }
};

// -----------------------------
// Conta (abstrata) e concretas mínimas
// -----------------------------
struct Conta : public enable_shared_from_this<Conta> {
    string numero;
    double saldo = 0.0;
    Extrato extratoFinanceiro;
    Extrato extratoPontos; // movimentacoes em pontos (positivas e negativas)
    vector<shared_ptr<Investimento>> investimentos;
    vector<shared_ptr<Cartao>> cartoes;
    vector<shared_ptr<class Cliente>> correntistas;

    Conta(string n=""): numero(n) {}
    virtual ~Conta() = default;
    virtual double calcularRendimentos(int dias) = 0;

    // Implementação pedida - retorna pontos atuais (soma ganhos - resgates)
    virtual int calcularPontos() {
        int pontos = 0;
        // 1) pontos por gastos nos cartões (todas as faturas)
        for (auto &cart : cartoes) {
            const auto& hf = cart->getHistoricoFaturas();
            for (auto &f : hf.getFaturas()) {
                for (auto &c : f->getCompras()) {
                    pontos += cart->calcularPontuacao(c->valor);
                }
            }
        }
        // 2) aplicar movimentos do extrato de pontos
        for (auto &m : extratoPontos.getMovs()) {
            pontos += static_cast<int>(m->valor); // votos: positivo ou negativo
        }
        return pontos;
    }
};

struct ContaCorrente : public Conta {
    ContaCorrente(string n=""): Conta(n) {}
    virtual double calcularRendimentos(int dias) override {
        return 0.0; 
    }
};

struct ContaPoupanca : public Conta {
    ContaPoupanca(string n=""): Conta(n) {}
    virtual double calcularRendimentos(int dias) override {
        // 0.5% + TR ao mês (1 mês = 30 dias)
        int meses = dias / 30;
        double tr = MercadoFinanceiro::getTR();
        double taxaMensal = 0.005 + tr; // 0.5% + TR
        // aplicar juros simples sobre saldo
        return this->saldo * (pow(1.0 + taxaMensal, meses) - 1.0);
    }
};

// -----------------------------
// Carteira::calcularVolumeGerenciado (implementação)
// -----------------------------
double Carteira::calcularVolumeGerenciado() {
    double total = 0.0;
    for (auto &c : contas) {
        total += c->saldo;
        for (auto &inv : c->investimentos) {
            // se for CDB use saldoLiquido (se disponível), senão saldoAtual
            if (auto cdb = dynamic_pointer_cast<CDB>(inv)) {
                total += cdb->saldoLiquido;
            } else {
                total += inv->saldoAtual;
            }
        }
    }
    return total;
}

// -----------------------------
// Cliente
// -----------------------------
struct Cliente : public Pessoa {
    vector<shared_ptr<Conta>> contas; // max 2 no enunciado, aqui controlamos informalmente

    Cliente(string n="", string cpf_=""): Pessoa(n, cpf_) {}
    void adicionarConta(const shared_ptr<Conta>& c) {
        // apenas vincula a conta ao cliente; para adicionar o cliente como correntista
        // use ClienteShared::adicionarContaSafe que utiliza shared_from_this corretamente.
        contas.push_back(c);
    }

    // Imprime lista ordenada de estabelecimentos por volume decrescente
    void imprimirListaDeEstabelecimentosFavoritos() {
        unordered_map<string, double> gastos;
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

        vector<pair<string,double>> lista;
        lista.reserve(gastos.size());
        for (auto &p : gastos) lista.emplace_back(p.first, p.second);

        sort(lista.begin(), lista.end(), [](const auto &a, const auto &b){
            return a.second > b.second;
        });

        Console::section("Estabelecimentos favoritos (por valor gasto)");
        for (auto &it : lista) {
            Console::kv(it.first, Console::money(it.second));
        }
    }
};

// To allow Cliente::adicionarConta to push a shared_ptr to correntistas vector,
// make Cliente inherit enable_shared_from_this:
struct ClienteShared : public Cliente, public enable_shared_from_this<ClienteShared> {
    ClienteShared(string n="", string cpf_=""): Cliente(n,cpf_) {}
    void adicionarContaSafe(const shared_ptr<Conta>& c) {
        contas.push_back(c);
        c->correntistas.push_back(static_pointer_cast<Cliente>(shared_from_this()));
    }
};

// -----------------------------
// MAIN: pequeno teste demonstrativo
// -----------------------------
int main() {
    // configurar mercado (valores de exemplo)
    MercadoFinanceiro::DI = 0.01; // 1% (exemplo hipotético)
    MercadoFinanceiro::TR = 0.002; // 0.2%

    // criar cliente
    auto cliente = make_shared<ClienteShared>("Thiago", "000.000.000-00");

    // criar conta e vincular ao cliente
    auto conta = make_shared<ContaPoupanca>("0001");
    conta->saldo = 1000.0;
    cliente->adicionarContaSafe(conta);

    // criar cartão Black e fatura com compras
    auto cartao = make_shared<Black>();
    auto f1 = make_shared<Fatura>();
    f1->adicionarCompra(make_shared<Compra>("Supermercado A", Data(2025,11,1), 250.0));
    f1->adicionarCompra(make_shared<Compra>("Padaria X", Data(2025,11,2), 30.0));
    cartao->historico.adicionarFatura(f1);
    conta->cartoes.push_back(cartao);

    // criar outra fatura em outro cartão
    auto cartao2 = make_shared<Platinum>();
    auto f2 = make_shared<Fatura>();
    f2->adicionarCompra(make_shared<Compra>("Loja B", Data(2025,10,25), 400.0));
    cartao2->historico.adicionarFatura(f2);
    conta->cartoes.push_back(cartao2);

    // adicionar movimentos de pontos (resgate)
    conta->extratoPontos.adicionarMov(make_shared<Movimentacao>(Data(2025,11,5), -50.0)); // resgatou 50 pontos

    // criar CDB investido na conta
    auto cdb = make_shared<CDB>();
    cdb->saldoAtual = 5000.0;
    cdb->taxaDiariaPercentualDI = 0.5; // 50% do DI por dia (exemplo exagerado)
    cdb->dataInicio = Data(2025,1,1);
    cdb->dataVencimento = Data(2026,1,1);
    conta->investimentos.push_back(cdb);

    // mostrar pontos antes de atualizar anything
    Console::section("Resumo de Pontos");
    Console::kv("Pontos (antes)", to_string(conta->calcularPontos()));

    // atualizar CDB (aplica rendimento e atualiza saldoLiquido)
    cdb->atualizarSaldo();

    Console::header("CDB");
    Console::kv("Saldo Atual", Console::money(cdb->saldoAtual));
    Console::kv("Saldo Líquido", Console::money(cdb->saldoLiquido));

    // calcular volume gerenciado na carteira do gerente (ou criar carteira)
    auto carteira = make_shared<Carteira>();
    carteira->adicionarConta(conta);
    Console::section("Carteira");
    Console::kv("Volume gerenciado", Console::money(carteira->calcularVolumeGerenciado()));

    // imprimir estabelecimentos favoritos do cliente
    cliente->imprimirListaDeEstabelecimentosFavoritos();

    // mostrar pontos finais (sem mudanças nos pontos do CDB)
    Console::kv("Pontos (depois)", to_string(conta->calcularPontos()));

    return 0;
}
