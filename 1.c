#include <stdio.h>
#include <gmp.h>

int main() {
    // Initialize variables
    mpz_t n, m, result;
    mpz_inits(n, m, result, NULL);

    printf("Enter a large integer: ");
    if (gmp_scanf("%Zd", n) != 1) return 1; 

    printf("Enter a Second Large Integer: ");
    if (gmp_scanf("%Zd", m) != 1) return 1; 

    mpz_mul(result, n, m);
    
    mpz_mul_ui(result, result, 2); 
   
    gmp_printf("Result (n * m * 2) = %Zd\n", result);

    // Cleanup
    mpz_clears(n, m, result, NULL);
    return 0;
}
