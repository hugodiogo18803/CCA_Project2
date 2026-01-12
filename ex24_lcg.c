/*
 * Exercício 24: Gerador de Números Pseudo-Aleatórios (LCG)
 * Fórmula: X_next = (a * X_prev + c) % m
 * Requisitos:
 * - Ler a, c, seed (e m) de ficheiro de texto.
 * - Ciclo com opções.
 * - Gerar N valores para ficheiro de saída.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

typedef struct
{
    mpz_t m; // Modulo
    mpz_t a; // Multiplicador
    mpz_t c; // Incremento
    mpz_t x; // Estado atual (Semente)
} LCG_State;

void init_lcg(LCG_State *state)
{
    mpz_inits(state->m, state->a, state->c, state->x, NULL);
}

void clear_lcg(LCG_State *state)
{
    mpz_clears(state->m, state->a, state->c, state->x, NULL);
}

// Carrega parametros do ficheiro de configuração
int carregar_config(LCG_State *state, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("Erro ao abrir ficheiro de configuracao");
        return 0;
    }

    // Leitura sequencial: m, a, c, x0
    int l1 = gmp_fscanf(f, "%Zd", state->m);
    int l2 = gmp_fscanf(f, "%Zd", state->a);
    int l3 = gmp_fscanf(f, "%Zd", state->c);
    int l4 = gmp_fscanf(f, "%Zd", state->x);

    fclose(f);

    if (l1 != 1 || l2 != 1 || l3 != 1 || l4 != 1)
    {
        fprintf(stderr, "Erro: Ficheiro mal formatado. Deve ter 4 linhas (m, a, c, seed).\n");
        return 0;
    }
    return 1;
}

// Passo do LCG: x = (a*x + c) % m
void lcg_next(LCG_State *state)
{
    mpz_mul(state->x, state->x, state->a); // x = x * a
    mpz_add(state->x, state->x, state->c); // x = x + c
    mpz_mod(state->x, state->x, state->m); // x = x % m
}

void gerar_valores()
{
    char config_file[256];
    char out_file[256];
    unsigned long qtd;
    LCG_State lcg;

    printf("\n Gerar Pseudo-Aleatorios (LCG) \n");

    // Configuração
    printf("Nome do ficheiro de configuracao (ex: lcg_config.txt): ");
    scanf("%255s", config_file);

    init_lcg(&lcg);
    if (!carregar_config(&lcg, config_file))
    {
        clear_lcg(&lcg);
        return;
    }

    // Parâmetros de Saída
    printf("Quantos valores deseja gerar? ");
    if (scanf("%lu", &qtd) != 1)
    {
        clear_lcg(&lcg);
        return;
    }

    printf("Nome do ficheiro de saida: ");
    scanf("%255s", out_file);

    // Execução
    FILE *f_out = fopen(out_file, "w");
    if (!f_out)
    {
        perror("Erro ao criar ficheiro de saida");
        clear_lcg(&lcg);
        return;
    }

    printf("A gerar %lu valores...\n", qtd);
    for (unsigned long i = 0; i < qtd; i++)
    {
        lcg_next(&lcg);
        gmp_fprintf(f_out, "%Zd\n", lcg.x);
    }

    fclose(f_out);
    clear_lcg(&lcg);
    printf("Sucesso. Valores guardados em '%s'.\n", out_file);
}

int main()
{
    int opcao = 1;

    printf("=== Exercicio 24: Gerador LCG ===\n");

    while (opcao)
    {
        printf("\n1. Gerar Aleatorios para Ficheiro\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
            break;

        if (opcao == 1)
        {
            gerar_valores();
        }
    }

    return 0;
}