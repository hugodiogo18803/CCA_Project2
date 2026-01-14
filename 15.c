#include <stdio.h>
#include <gmp.h>
#include <time.h> 
#include <stdlib.h>

void gerar_primo(mpz_t p, unsigned long bits, gmp_randstate_t state)
{
    mpz_t rand_num;
    mpz_init(rand_num);

    mpz_urandomb(rand_num, state, bits);

    mpz_setbit(rand_num, 0);
    mpz_setbit(rand_num, bits - 1);

    mpz_nextprime(p, rand_num);

    mpz_clear(rand_num);
}

int main()
{
    mpz_t p, q, n, phi, e, d, p_minus_1, q_minus_1;
    unsigned long key_size_bits;
    
    // Variáveis para medição de tempo
    clock_t inicio, fim;
    double tempo_primos, tempo_total;

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_inits(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);

    printf("=== Exercicio 14 (Modificado): RSA com Medicao de Tempo ===\n");
    printf("Introduza a dimensao da chave em bits (ex: 2048, 4096): ");
    if (scanf("%lu", &key_size_bits) != 1)
        return 1;

    // --- Início da medição total ---
    clock_t tempo_inicio_global = clock();

    printf("\n[1/5] A gerar primos p e q...\n");
    inicio = clock(); 
    
    gerar_primo(p, key_size_bits / 2, state);
    gerar_primo(q, key_size_bits / 2, state);

    while (mpz_cmp(p, q) == 0)
    {
        gerar_primo(q, key_size_bits / 2, state);
    }
    
    fim = clock();
    tempo_primos = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("[2/5] A calcular Modulo n...\n");
    mpz_mul(n, p, q);

    printf("[3/5] A calcular Totiente phi(n)...\n");
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    printf("[4/5] A definir expoente publico e...\n");
    mpz_set_ui(e, 65537);
    mpz_t gcd_res;
    mpz_init(gcd_res);
    while (1)
    {
        mpz_gcd(gcd_res, e, phi);
        if (mpz_cmp_ui(gcd_res, 1) == 0) break;
        mpz_add_ui(e, e, 2);
    }
    mpz_clear(gcd_res);

    printf("[5/5] A calcular expoente privado d (Inverso Modular)...\n");
    inicio = clock();
    
    if (mpz_invert(d, e, phi) == 0)
    {
        fprintf(stderr, "ERRO: Inverso modular nao existe.\n");
        return 1;
    }
    
    fim = clock();
    double tempo_d = (double)(fim - inicio) / CLOCKS_PER_SEC;

    clock_t tempo_fim_global = clock();
    tempo_total = (double)(tempo_fim_global - tempo_inicio_global) / CLOCKS_PER_SEC;

    // --- Apresentação dos Resultados ---
    printf("\n--- Relatorio de Performance ---\n");
    printf("Tempo para gerar primos:   %.6f segundos\n", tempo_primos);
    printf("Tempo para calcular 'd':   %.6f segundos\n", tempo_d);
    printf("Tempo TOTAL de geracao:    %.6f segundos\n", tempo_total);

    printf("\n=== Chaves Geradas ===\n");
    gmp_printf("Publica (e): %Zd\n", e);
    gmp_printf("Privada (d): %Zd\n", d);
    gmp_printf("Modulo (n):  %Zd\n", n);

    mpz_clears(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);
    gmp_randclear(state);

    return 0;
}