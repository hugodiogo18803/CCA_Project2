/*
 * Exercício 12: Fatorização de Combinações de Primos
 * Requisitos:
 * - Listar Primos < Limite.
 * - Gerar combinações de primos 2 a 2.
 * - Gravar "resultado de uma factorização" por linha num ficheiro.
 * - Input: Limite e Nome do Ficheiro (User).
 * - Ciclo de execução.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Estrutura para array dinâmico de mpz_t
typedef struct
{
    mpz_t *elementos;
    size_t tamanho;
    size_t capacidade;
} ListaPrimos;

void iniciar_lista(ListaPrimos *l)
{
    l->tamanho = 0;
    l->capacidade = 100;
    l->elementos = malloc(l->capacidade * sizeof(mpz_t));
}

void adicionar_primo(ListaPrimos *l, mpz_t p)
{
    if (l->tamanho == l->capacidade)
    {
        l->capacidade *= 2;
        l->elementos = realloc(l->elementos, l->capacidade * sizeof(mpz_t));
    }
    mpz_init_set(l->elementos[l->tamanho], p);
    l->tamanho++;
}

void limpar_lista(ListaPrimos *l)
{
    for (size_t i = 0; i < l->tamanho; i++)
    {
        mpz_clear(l->elementos[i]);
    }
    free(l->elementos);
    l->tamanho = 0;
    l->capacidade = 0;
}

int main()
{
    mpz_t limite, iterador, produto;
    char nome_ficheiro[256];
    int opcao = 1;

    mpz_inits(limite, iterador, produto, NULL);

    printf("=== Exercicio 12: Fatorizacao de Combinacoes ===\n");

    while (opcao)
    {
        printf("\n--- Novo Processamento ---\n");

        // 1. Inputs
        printf("Introduza o limite maximo: ");
        if (gmp_scanf("%Zd", limite) != 1)
            break;

        printf("Introduza o nome do ficheiro de saida: ");
        if (scanf("%255s", nome_ficheiro) != 1)
            break;

        // 2. Recolher Primos para Memória
        printf("A gerar lista de primos inferiores a ");
        gmp_printf("%Zd", limite);
        printf("...\n");

        ListaPrimos lista;
        iniciar_lista(&lista);

        mpz_set_ui(iterador, 2); // Primeiro primo

        while (mpz_cmp(iterador, limite) < 0)
        {
            adicionar_primo(&lista, iterador);
            mpz_nextprime(iterador, iterador);
        }

        printf("Encontrados %zu primos. A gerar combinacoes e a escrever no ficheiro...\n", lista.tamanho);

        // 3. Gerar Combinações e Escrever
        FILE *f = fopen(nome_ficheiro, "w");
        if (!f)
        {
            perror("Erro ao abrir ficheiro");
        }
        else
        {
            // Combinações 2 a 2 (Nested Loop)
            // i vai de 0 a N-1
            // j vai de i+1 a N-1 (para evitar repetições e auto-multiplicação se for estrito)
            for (size_t i = 0; i < lista.tamanho; i++)
            {
                for (size_t j = i + 1; j < lista.tamanho; j++)
                {

                    // N = P * Q
                    mpz_mul(produto, lista.elementos[i], lista.elementos[j]);

                    // Formato: "N = P * Q"
                    gmp_fprintf(f, "%Zd = %Zd * %Zd\n",
                                produto,
                                lista.elementos[i],
                                lista.elementos[j]);
                }
            }
            fclose(f);
            printf("Sucesso! Ficheiro '%s' gerado.\n", nome_ficheiro);
        }

        // 4. Limpeza da Lista Dinâmica
        limpar_lista(&lista);

        printf("\nContinuar? (1=Sim, 0=Nao): ");
        scanf("%d", &opcao);
    }

    mpz_clears(limite, iterador, produto, NULL);
    return 0;
}