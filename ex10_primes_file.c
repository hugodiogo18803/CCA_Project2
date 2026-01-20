/*
 * Exercício 10: Listagem de Primos para Ficheiro
 * Requisitos:
 * - Limite máximo (argv[1]).
 * - Nome do ficheiro de saída (argv[2]).
 * - Validação de argumentos.
 * - Execução única.
 */

#include <stdio.h>
#include <gmp.h>

void print_usage(const char *prog_name)
{
    printf("Uso incorreto.\n");
    printf("Sintaxe: %s <LIMIT_MAXIMO> <NOME_FICHEIRO>\n", prog_name);
    printf("Exemplo: %s 1000 primos.txt\n", prog_name);
}

int main(int argc, char *argv[])
{
    // Validação de Argumentos (Esperados 3: programa, limite, ficheiro)
    if (argc != 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    mpz_t limite, iterador;
    mpz_inits(limite, iterador, NULL);

    // Processar Limite
    if (mpz_set_str(limite, argv[1], 10) != 0)
    {
        fprintf(stderr, "Erro: O limite '%s' nao e valido.\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    // Abrir Ficheiro
    FILE *f = fopen(argv[2], "w");
    if (f == NULL)
    {
        perror("Erro ao criar ficheiro"); // Imprime o erro do sistema (ex: permissão negada)
        return 1;
    }

    printf("A escrever primos menores que %s em '%s'...\n", argv[1], argv[2]);

    // Loop e Escrita em Ficheiro
    mpz_set_ui(iterador, 2); // Primeiro primo

    while (mpz_cmp(iterador, limite) < 0)
    {
        // gmp_fprintf funciona como fprintf mas suporta tipos GMP
        gmp_fprintf(f, "%Zd\n", iterador);

        mpz_nextprime(iterador, iterador);
    }

    // Fecho e Limpeza
    fclose(f);
    mpz_clears(limite, iterador, NULL);

    printf("Concluido.\n");

    return 0;
}