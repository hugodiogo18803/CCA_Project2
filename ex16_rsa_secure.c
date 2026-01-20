/*
 * Exercício 16: Melhoria da Geração de Aleatórios (Seed Segura)
 * Requisitos:
 * - Substituir time(NULL) por leitura de /dev/urandom.
 * - Gerar chaves RSA com base nesta entropia superior.
 */

#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

// Função para obter uma semente segura do sistema operativo
unsigned long get_secure_seed()
{
    unsigned long seed;
    FILE *f = fopen("/dev/urandom", "rb");

    if (!f)
    {
        fprintf(stderr, "ERRO FATAL: Nao foi possivel abrir /dev/urandom\n");
        exit(1);
    }

    // Ler bytes aleatórios suficientes para encher um unsigned long
    if (fread(&seed, sizeof(seed), 1, f) != 1)
    {
        fprintf(stderr, "ERRO FATAL: Falha ao ler entropia.\n");
        fclose(f);
        exit(1);
    }

    fclose(f);
    return seed;
}

void gerar_primo(mpz_t p, unsigned long bits, gmp_randstate_t state)
{
    mpz_t rand_num;
    mpz_init(rand_num);
    mpz_urandomb(rand_num, state, bits);
    mpz_setbit(rand_num, 0);        // Ímpar
    mpz_setbit(rand_num, bits - 1); // Magnitude
    mpz_nextprime(p, rand_num);
    mpz_clear(rand_num);
}

int main()
{
    mpz_t p, q, n, phi, e, d, pm1, qm1, gcd_res;
    unsigned long key_bits;
    gmp_randstate_t state;

    // Inicializar o gerador (Mersenne Twister é o default)
    gmp_randinit_default(state);

    // Obter semente criptograficamente forte
    unsigned long seed = get_secure_seed();
    printf("Semente de entropia obtida: %lu\n", seed);

    // Semear o gerador
    gmp_randseed_ui(state, seed);

    mpz_inits(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);

    printf("=== Exercicio 16: RSA com Entropia Segura (/dev/urandom) ===\n");
    printf("Dimensao da chave (bits): ");
    if (scanf("%lu", &key_bits) != 1)
        return 1;

    printf("A gerar chaves... (Isto e mais seguro e imprevisivel)\n");

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
        fprintf(stderr, "Erro no inverso modular.\n");
        return 1;
    }

    printf("\n--- Chave Publica ---\n");
    gmp_printf("e: %Zd\nn: %Zd\n", e, n);

    printf("\n--- Chave Privada ---\n");
    gmp_printf("d: %Zd\n", d);

    mpz_clears(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);
    gmp_randclear(state);

    return 0;
}