/*
 * Exercício 13: Fatorização de Combinações de Primos (Formato Simples)
 * Requisitos:
 * - Listar Primos < Limite.
 * - Gerar combinações de primos 2 a 2.
 * - Gravar no ficheiro: "Produto Primo1 Primo2" por linha.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

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

    printf("=== Exercicio 13: Fatorizacao (Tres Numeros por Linha) ===\n");

    while (opcao)
    {
        printf("\n--- Novo Processamento ---\n");

        printf("Introduza o limite maximo: ");
        if (gmp_scanf("%Zd", limite) != 1)
            break;

        printf("Introduza o nome do ficheiro de saida: ");
        if (scanf("%255s", nome_ficheiro) != 1)
            break;

        printf("A gerar lista de primos...\n");

        ListaPrimos lista;
        iniciar_lista(&lista);

        mpz_set_ui(iterador, 2); 

        while (mpz_cmp(iterador, limite) < 0)
        {
            adicionar_primo(&lista, iterador);
            mpz_nextprime(iterador, iterador);
        }

        printf("Encontrados %zu primos. A escrever no ficheiro...\n", lista.tamanho);

        FILE *f = fopen(nome_ficheiro, "w");
        if (!f)
        {
            perror("Erro ao abrir ficheiro");
        }
        else
        {
            for (size_t i = 0; i < lista.tamanho; i++)
            {
                for (size_t j = i + 1; j < lista.tamanho; j++)
                {
                    // Calcula o produto (fatorização)
                    mpz_mul(produto, lista.elementos[i], lista.elementos[j]);

                    // FORMATO EXERCÍCIO 13: Produto Primo1 Primo2
                    gmp_fprintf(f, "%Zd %Zd %Zd\n",
                                produto,
                                lista.elementos[i],
                                lista.elementos[j]);
                }
            }
            fclose(f);
            printf("Sucesso! Ficheiro '%s' gerado.\n", nome_ficheiro);
        }

        limpar_lista(&lista);

        printf("\nContinuar? (1=Sim, 0=Nao): ");
        if (scanf("%d", &opcao) != 1) opcao = 0;
    }

    mpz_clears(limite, iterador, produto, NULL);
    return 0;
}