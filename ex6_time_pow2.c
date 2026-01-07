/*
 * Exercício 6: Comparação de tempos na exponenciação de base 2.
 * Requisitos:
 * - Comparar soluções "com e sem números de grande dimensão" (interpretado como
 * comparar algoritmos eficientes vs ineficientes em várias escalas).
 * - Apenas potências de 2.
 * - Medição de tempo (clock).
 */

#include <stdio.h>
#include <gmp.h>
#include <time.h>

// Função auxiliar para medir e imprimir
void medir_tempo(const char *nome_algo, clock_t inicio, clock_t fim)
{
    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("   [ %-20s ]: %.6f segundos\n", nome_algo, tempo_cpu);
}

int main()
{
    mpz_t base, resultado;
    unsigned long k; // Expoente (usamos unsigned long para simplificar input)
    int opcao = 1;

    mpz_inits(base, resultado, NULL);

    printf("=== Exercicio 6: Performance em Potencias de 2 ===\n");
    printf("Comparacao entre Shift (Otimizado), Pow (Generico) e Iterativo (Lento)\n");

    while (opcao)
    {
        printf("\n--- Novo Teste ---\n");
        printf("Introduza o expoente k para calcular 2^k: ");
        if (scanf("%lu", &k) != 1)
        {
            // Limpar buffer se input for invalido
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }

        printf("\nCalculando 2^%lu (Numero de bits resultante: aprox %lu)...\n", k, k);

        // Metodo 1: Bitwise Shift (mpz_mul_2exp)
        // Equivalente a 1 << k. É instantâneo pois apenas define o bit k.
        mpz_set_ui(base, 1); // Reset base a 1

        clock_t t1 = clock();
        mpz_mul_2exp(resultado, base, k);
        clock_t t2 = clock();

        medir_tempo("GMP Bit Shift", t1, t2);

        // Metodo 2: Exponenciação Genérica (mpz_pow_ui)
        // Calcula base^exp usando algoritmos matemáticos padrão
        mpz_set_ui(base, 2); // Base 2

        t1 = clock();
        mpz_pow_ui(resultado, base, k);
        t2 = clock();

        medir_tempo("GMP Generic Pow", t1, t2);

        // Metodo 3: Multiplicação Iterativa (Loop)
        // Apenas executamos se k for 'pequeno' (< 200,000) para nao bloquear o PC
        if (k <= 200000)
        {
            mpz_set_ui(base, 2);
            mpz_set_ui(resultado, 1);

            t1 = clock();
            for (unsigned long i = 0; i < k; i++)
            {
                mpz_mul(resultado, resultado, base); // res = res * 2
            }
            t2 = clock();
            medir_tempo("Ciclo Naive (x2)", t1, t2);
        }
        else
        {
            printf("   [ %-20s ]: IGNORADO (Lento demais para k=%lu)\n", "Ciclo Naive (x2)", k);
        }

        // Menu de Saida
        printf("\nNovo teste? (1=Sim, 0=Nao): ");
        if (scanf("%d", &opcao) != 1)
            opcao = 0;
    }

    mpz_clears(base, resultado, NULL);
    return 0;
}