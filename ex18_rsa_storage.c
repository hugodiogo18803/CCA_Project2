/*
 * Exercício 18: Gravação de Chaves RSA em Ficheiro
 * Baseado no Ex 16 (Geração Segura).
 * Adiciona rotinas de I/O para secret.myasc e public.myasc.
 */

#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

// Funções Auxiliares
unsigned long get_secure_seed()
{
    unsigned long seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if (!f)
        exit(1);
    if (fread(&seed, sizeof(seed), 1, f) != 1)
        exit(1);
    fclose(f);
    return seed;
}

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
    mpz_t p, q, n, phi, e, d, pm1, qm1, gcd_res;
    unsigned long key_bits;
    gmp_randstate_t state;

    // 1. Setup RSA Seguro
    gmp_randinit_default(state);
    gmp_randseed_ui(state, get_secure_seed());
    mpz_inits(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);

    printf("=== Exercicio 18: RSA Key Storage ===\n");
    printf("Dimensao da chave (bits): ");
    if (scanf("%lu", &key_bits) != 1)
        return 1;

    printf("A gerar chaves...\n");

    // Geração
    gerar_primo(p, key_bits / 2, state);
    gerar_primo(q, key_bits / 2, state);
    while (mpz_cmp(p, q) == 0)
        gerar_primo(q, key_bits / 2, state);

    mpz_mul(n, p, q);

    mpz_sub_ui(pm1, p, 1);
    mpz_sub_ui(qm1, q, 1);
    mpz_mul(phi, pm1, qm1);

    mpz_set_ui(e, 65537);
    while (1)
    {
        mpz_gcd(gcd_res, e, phi);
        if (mpz_cmp_ui(gcd_res, 1) == 0)
            break;
        mpz_add_ui(e, e, 2);
    }

    if (mpz_invert(d, e, phi) == 0)
    {
        fprintf(stderr, "Erro critico: Inverso modular falhou.\n");
        return 1;
    }

    // Gravação: public.myasc (Apenas Publica)
    // Formato: e,n
    FILE *f_pub = fopen("public.myasc", "w");
    if (f_pub)
    {
        gmp_fprintf(f_pub, "%Zd,%Zd", e, n);
        fclose(f_pub);
        printf("[OK] Chave publica guardada em 'public.myasc'.\n");
    }
    else
    {
        perror("Erro ao criar public.myasc");
    }

    // Gravação: secret.myasc (Par de Chaves)
    // Formato:
    // e,n
    // (linha vazia)
    // d,n
    FILE *f_sec = fopen("secret.myasc", "w");
    if (f_sec)
    {
        // Linha 1: Publica
        gmp_fprintf(f_sec, "%Zd,%Zd\n", e, n);
        // Linha 2: Vazia
        fprintf(f_sec, "\n");
        // Linha 3: Privada
        gmp_fprintf(f_sec, "%Zd,%Zd", d, n);

        fclose(f_sec);
        printf("[OK] Par de chaves guardado em 'secret.myasc'.\n");
    }
    else
    {
        perror("Erro ao criar secret.myasc");
    }

    // Limpeza
    mpz_clears(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);
    gmp_randclear(state);

    return 0;
}