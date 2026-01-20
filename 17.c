#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Função auxiliar para gerar primos (necessária para a fase de geração)
void gerar_primo(mpz_t p, unsigned long bits, gmp_randstate_t state) {
    mpz_t rand_num;
    mpz_init(rand_num);
    mpz_urandomb(rand_num, state, bits);
    mpz_setbit(rand_num, 0);
    mpz_setbit(rand_num, bits - 1);
    mpz_nextprime(p, rand_num);
    mpz_clear(rand_num);
}

int main() {
    mpz_t p, q, n, phi, e, d, msg, cifra, decifra, pm1, qm1, gcd_res;
    unsigned long tamanhos[] = {1024, 2048, 4096};
    gmp_randstate_t state;
    
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_inits(p, q, n, phi, e, d, msg, cifra, decifra, pm1, qm1, gcd_res, NULL);
    mpz_set_ui(msg, 12345); // Mensagem de teste

    printf("Bits | Geracao(s) | Cifra(s) | Decifra(s)\n");
    printf("-------------------------------------------\n");

    for(int i = 0; i < 3; i++) {
        unsigned long b = tamanhos[i];

        // 1. TEMPO DE GERAÇÃO
        clock_t t1 = clock();
        gerar_primo(p, b/2, state);
        gerar_primo(q, b/2, state);
        mpz_mul(n, p, q);
        mpz_sub_ui(pm1, p, 1); mpz_sub_ui(qm1, q, 1);
        mpz_mul(phi, pm1, qm1);
        mpz_set_ui(e, 65537);
        mpz_invert(d, e, phi);
        double tempo_gen = (double)(clock() - t1) / CLOCKS_PER_SEC;

        // 2. TEMPO DE CIFRA
        clock_t t2 = clock();
        mpz_powm(cifra, msg, e, n);
        double tempo_cifra = (double)(clock() - t2) / CLOCKS_PER_SEC;

        // 3. TEMPO DE DECIFRA
        clock_t t3 = clock();
        mpz_powm(decifra, cifra, d, n);
        double tempo_decifra = (double)(clock() - t3) / CLOCKS_PER_SEC;

        printf("%lu | %f | %f | %f\n", b, tempo_gen, tempo_cifra, tempo_decifra);
    }

    mpz_clears(p, q, n, phi, e, d, msg, cifra, decifra, pm1, qm1, gcd_res, NULL);
    gmp_randclear(state);
    return 0;
}