/*
 * Exercício 14: Geração de Chaves RSA
 * Requisitos:
 * - Dimensão escolhida pelo utilizador (em bits).
 * - Gerar chaves Publica (e, n) e Privada (d, n).
 * - Utilizar GMP para tudo.
 */

#include <stdio.h>
#include <gmp.h>
#include <time.h> // Para a semente aleatória
#include <stdlib.h>

// Função auxiliar para gerar um primo de tamanho 'bits'
void gerar_primo(mpz_t p, unsigned long bits, gmp_randstate_t state)
{
    mpz_t rand_num;
    mpz_init(rand_num);

    // 1. Gerar número aleatório com o número de bits pedido
    mpz_urandomb(rand_num, state, bits);

    // 2. Garantir que é impar (bit 0 a 1) e tem a magnitude certa (bit mais alto a 1)
    mpz_setbit(rand_num, 0);
    mpz_setbit(rand_num, bits - 1);

    // 3. Encontrar o próximo primo a partir deste número aleatório
    mpz_nextprime(p, rand_num);

    mpz_clear(rand_num);
}

int main()
{
    mpz_t p, q, n, phi, e, d, p_minus_1, q_minus_1;
    unsigned long key_size_bits;

    // Inicialização do estado aleatório
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)); // Semente baseada no relógio

    mpz_inits(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);

    printf("=== Exercicio 14: Gerador de Chaves RSA ===\n");
    printf("Introduza a dimensao da chave em bits (ex: 1024, 2048): ");
    if (scanf("%lu", &key_size_bits) != 1)
        return 1;

    printf("\n[1/5] A gerar primos p e q (pode demorar)...\n");

    // Gerar p e q com metade do tamanho da chave final
    gerar_primo(p, key_size_bits / 2, state);
    gerar_primo(q, key_size_bits / 2, state);

    // Garantir que p != q (extremamente raro ser igual, mas boa prática)
    while (mpz_cmp(p, q) == 0)
    {
        gerar_primo(q, key_size_bits / 2, state);
    }

    printf("[2/5] A calcular Modulo n = p * q...\n");
    mpz_mul(n, p, q); // n = p * q

    printf("[3/5] A calcular Totiente phi(n) = (p-1)*(q-1)...\n");
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    printf("[4/5] A definir expoente publico e = 65537...\n");
    mpz_set_ui(e, 65537);

    // Verificar se e é coprimo com phi. Se não for, incrementa e.
    mpz_t gcd_res;
    mpz_init(gcd_res);
    while (1)
    {
        mpz_gcd(gcd_res, e, phi);
        if (mpz_cmp_ui(gcd_res, 1) == 0)
            break;           // GCD é 1, válido
        mpz_add_ui(e, e, 2); // Tenta o próximo ímpar
    }
    mpz_clear(gcd_res);

    printf("[5/5] A calcular expoente privado d (inverso modular)...\n");
    // d = e^-1 mod phi
    if (mpz_invert(d, e, phi) == 0)
    {
        fprintf(stderr, "ERRO CRITICO: Inverso modular nao existe.\n");
        return 1;
    }

    printf("\n=== Chaves Geradas ===\n");
    printf("Chave Publica (e, n):\n");
    gmp_printf("  e: %Zd\n", e);
    gmp_printf("  n: %Zd\n", n);

    printf("\nChave Privada (d, n) [MANTER SECRETA!]:\n");
    gmp_printf("  d: %Zd\n", d);

    // Limpeza
    mpz_clears(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);
    gmp_randclear(state);

    return 0;
}