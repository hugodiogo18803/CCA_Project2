/*
 * Exercício 8: Listagem de Primos com Limite via Argumento
 * Requisitos:
 * - Limite máximo enviado como 1.º parâmetro (argv[1]).
 * - Validar passagem de parâmetros e disponibilizar ajuda.
 * - Execução única (sem ciclo).
 */

#include <stdio.h>
#include <gmp.h>

void print_usage(const char *prog_name)
{
    printf("Uso incorreto.\n");
    printf("Sintaxe: %s <LIMIT_MAXIMO>\n", prog_name);
    printf("Exemplo: %s 100\n", prog_name);
}

int main(int argc, char *argv[])
{
    // Validação de Argumentos
    // argc deve ser 2 (o nome do programa + 1 argumento)
    if (argc != 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    mpz_t limite, iterador;
    mpz_inits(limite, iterador, NULL);

    // Processamento do Argumento (String -> mpz_t)
    // mpz_set_str retorna 0 em sucesso, -1 em erro. Base 10.
    if (mpz_set_str(limite, argv[1], 10) != 0)
    {
        fprintf(stderr, "Erro: O argumento '%s' nao e um numero valido.\n", argv[1]);
        print_usage(argv[0]);
        mpz_clears(limite, iterador, NULL);
        return 1;
    }

    printf("=== Exercicio 8: Primos inferiores a %s ===\n", argv[1]);

    // Algoritmo de Listagem
    // Começamos no primeiro primo: 2
    mpz_set_ui(iterador, 2);

    // Enquanto iterador < limite
    while (mpz_cmp(iterador, limite) < 0)
    {
        // Imprimir o primo atual
        gmp_printf("%Zd\n", iterador);

        // Calcular o próximo número primo (Otimizado pela GMP)
        // Isto evita testar todos os números pares ou não primos intermédios
        mpz_nextprime(iterador, iterador);
    }

    // Limpeza
    mpz_clears(limite, iterador, NULL);
    return 0;
}