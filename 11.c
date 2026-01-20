#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t result;
    unsigned long n, k;

    mpz_init(result);

    printf("--- Calculadora de Combinações (nCr) ---\n");

    printf("Introduza o número total de elementos (n): ");
    if (scanf("%lu", &n) != 1) {
        printf("Erro na leitura de n.\n");
        return 1;
    }

    printf("Introduza o número de elementos por grupo (k): ");
    if (scanf("%lu", &k) != 1) {
        printf("Erro na leitura de k.\n");
        return 1;
    }

    if (k > n) {
        printf("Resultado: 0 (Não é possível agrupar %lu elementos se só tem %lu).\n", k, n);
    } else {
        mpz_bin_uiui(result, n, k);

        gmp_printf("\nO numero de combinacoes possiveis e:\n%Zd\n", result);
    }

    mpz_clear(result);

    return 0;
}