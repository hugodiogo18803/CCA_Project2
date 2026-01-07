/*
 * Exercício 4: Exponenciação Modular
 * Requisitos:
 * - Calcular base^exp % mod
 * - Funcionar em ciclo até ordem de saída
 */

#include <stdio.h>
#include <gmp.h>

int main()
{
    mpz_t base, expoente, modulo, resultado;
    mpz_inits(base, expoente, modulo, resultado, NULL);

    int opcao = 1;

    printf("=== Exercicio 4: Exponenciacao Modular ===\n");

    while (opcao)
    { // Ciclo até ordem de saída

        printf("\n--- Novo Calculo ---\n");

        // 1. Leitura da Base
        printf("Introduza a Base: ");
        if (gmp_scanf("%Zd", base) != 1)
        {
            fprintf(stderr, "Erro de input (Base).\n");
            break;
        }

        // 2. Leitura do Expoente
        printf("Introduza o Expoente: ");
        if (gmp_scanf("%Zd", expoente) != 1)
        {
            fprintf(stderr, "Erro de input (Expoente).\n");
            break;
        }

        // 3. Leitura do Modulo
        // Essencial para criptografia e gestão de memória
        printf("Introduza o Modulo: ");
        if (gmp_scanf("%Zd", modulo) != 1)
        {
            fprintf(stderr, "Erro de input (Modulo).\n");
            break;
        }

        // Verificação de segurança: Modulo deve ser > 0
        if (mpz_cmp_ui(modulo, 0) <= 0)
        {
            printf("Erro: O modulo deve ser positivo.\n");
            continue;
        }

        // 4. Calculo: res = base^exp % mod
        // Função otimizada da GMP
        mpz_powm(resultado, base, expoente, modulo);

        // 5. Output
        printf("Resultado: ");
        gmp_printf("%Zd\n", resultado);

        // Menu de Saída
        printf("\nNovo calculo? (1=Sim, 0=Nao): ");
        if (scanf("%d", &opcao) != 1)
            opcao = 0;
    }

    mpz_clears(base, expoente, modulo, resultado, NULL);
    return 0;
}