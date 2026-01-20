#include <stdio.h>
#include <gmp.h>
#include <stdlib.h> 

int main() {
    mpz_t n;
    char inputString[1024];

    mpz_inits(n, NULL);
    
    printf("Enter a large integer: ");
    scanf("%1023s", inputString); 
   
    if (mpz_set_str(n, inputString, 10) == -1) {
        printf("That is not a valid number.\n");
        return 1;
    }

    int status = mpz_probab_prime_p(n, 25);

    if (status == 2) {
        printf("The number is definitely prime.\n");
    } else {
        printf("The number is not definitely prime.\n");
    }

    mpz_clears(n, NULL);
    return 0;
}
