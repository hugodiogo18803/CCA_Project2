/*
 * Exercício 2: Cálculo do GCD (Greatest Common Divisor)
 * Requisitos: Ler dois números inteiros de qualquer dimensão e calcular o MDC.
 */

#include <stdio.h>
#include <gmp.h>

int main()
{
    // 1. Declaração
    mpz_t n1, n2, resultado;

    // 2. Inicialização
    mpz_inits(n1, n2, resultado, NULL);

    printf("=== Exercicio 2: GCD (Maximo Divisor Comum) ===\n");

    // 3. Leitura dos inputs
    printf("Introduza o 1.o numero: ");
    if (gmp_scanf("%Zd", n1) != 1)
    {
        fprintf(stderr, "Erro na leitura.\n");
        return 1;
    }

    printf("Introduza o 2.o numero: ");
    if (gmp_scanf("%Zd", n2) != 1)
    {
        fprintf(stderr, "Erro na leitura.\n");
        return 1;
    }

    // 4. Cálculo do GCD
    // Sintaxe: mpz_gcd(variavel_destino, operando1, operando2)
    mpz_gcd(resultado, n1, n2);

    // 5. Output
    printf("\nO GCD de ");
    gmp_printf("%Zd", n1);
    printf(" e ");
    gmp_printf("%Zd", n2);
    printf(" e: ");
    gmp_printf("%Zd\n", resultado);

    // 6. Limpeza
    mpz_clears(n1, n2, resultado, NULL);

    return 0;
}