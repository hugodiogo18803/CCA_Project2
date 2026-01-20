/*
 * Exercício 13: Fatorização de Combinações de Primos (Formato Simples)
 * Objetivo: Gerar semiprimos (produto de dois primos) para testes de fatorização.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Estrutura para gerir dinamicamente um array de objetos mpz_t (GMP).
typedef struct
{
    mpz_t *elementos; // Ponteiro para o array de números grandes.
    size_t tamanho;   // Número atual de primos na lista.
    size_t capacidade;// Espaço total alocado em memória.
} ListaPrimos;

// Inicializa a estrutura de dados com uma capacidade base de 100.
void iniciar_lista(ListaPrimos *l)
{
    l->tamanho = 0;
    l->capacidade = 100;
    l->elementos = malloc(l->capacidade * sizeof(mpz_t)); // Alocação dinâmica.
}

// Adiciona um primo à lista e expande a memória (realloc) se necessário.
void adicionar_primo(ListaPrimos *l, mpz_t p)
{
    if (l->tamanho == l->capacidade)
    {
        l->capacidade *= 2; // Dobra a capacidade se estiver cheio.
        l->elementos = realloc(l->elementos, l->capacidade * sizeof(mpz_t));
    }
    // mpz_init_set inicializa e copia o valor do primo para a lista.
    mpz_init_set(l->elementos[l->tamanho], p);
    l->tamanho++;
}

// Liberta a memória de cada número individual e do array principal.
void limpar_lista(ListaPrimos *l)
{
    for (size_t i = 0; i < l->tamanho; i++)
    {
        mpz_clear(l->elementos[i]); // Limpa a memória interna do mpz_t.
    }
    free(l->elementos);
    l->tamanho = 0;
    l->capacidade = 0;
}

int main()
{
    // Declaração de variáveis GMP.
    mpz_t limite, iterador, produto;
    char nome_ficheiro[256];
    int opcao = 1;

    // Inicialização múltipla de variáveis GMP.
    mpz_inits(limite, iterador, produto, NULL);

    printf("=== Exercicio 13: Fatorizacao (Tres Numeros por Linha) ===\n");

    while (opcao)
    {
        printf("\n--- Novo Processamento ---\n");

        printf("Introduza o limite maximo: ");
        // gmp_scanf lê um número grande do teclado usando o formato %Zd.
        if (gmp_scanf("%Zd", limite) != 1)
            break;

        printf("Introduza o nome do ficheiro de saida: ");
        if (scanf("%255s", nome_ficheiro) != 1)
            break;

        printf("A gerar lista de primos...\n");

        ListaPrimos lista;
        iniciar_lista(&lista);

        mpz_set_ui(iterador, 2); // Define o início da busca de primos em 2.

        // Loop para encontrar todos os primos abaixo do limite definido.
        while (mpz_cmp(iterador, limite) < 0)
        {
            adicionar_primo(&lista, iterador);
            // mpz_nextprime encontra o próximo primo superior ao atual de forma eficiente.
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
            // Algoritmo combinatório: combina cada primo 'i' com todos os primos 'j' seguintes.
            for (size_t i = 0; i < lista.tamanho; i++)
            {
                for (size_t j = i + 1; j < lista.tamanho; j++)
                {
                    // Calcula o produto (semiprimo): produto = primo1 * primo2.
                    mpz_mul(produto, lista.elementos[i], lista.elementos[j]);

                    // Grava no ficheiro o formato: Produto Primo1 Primo2.
                    gmp_fprintf(f, "%Zd %Zd %Zd\n",
                                produto,
                                lista.elementos[i],
                                lista.elementos[j]);
                }
            }
            fclose(f);
            printf("Sucesso! Ficheiro '%s' gerado.\n", nome_ficheiro);
        }

        limpar_lista(&lista); // Liberta memória antes de um novo ciclo ou fecho.

        printf("\nContinuar? (1=Sim, 0=Nao): ");
        if (scanf("%d", &opcao) != 1) opcao = 0;
    }

    // Liberta a memória das variáveis globais do programa.
    mpz_clears(limite, iterador, produto, NULL);
    return 0;
}
