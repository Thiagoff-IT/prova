#include "include/calendario.h"
#include "include/console.h"
#include "include/extrato.h"
#include "include/compra.h"
#include "include/investimento.h"
#include "include/cartao.h"
#include "include/conta.h"
#include "include/pessoa.h"
#include "include/carteira.h"

#include <memory>
#include <string>

using namespace std;

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
    conta->extratoPontos.adicionarMov(make_shared<Movimentacao>(Data(2025,11,5), -50.0));

    // criar CDB investido na conta
    auto cdb = make_shared<CDB>();
    cdb->saldoAtual = 5000.0;
    cdb->taxaDiariaPercentualDI = 0.5;
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
