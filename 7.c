#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h> // Required for strcmp

int main() {
    mpz_t n, limit, current;
    char inputString[1024];

   
    mpz_inits(n, limit, current, NULL);

    while(1) 
    {
        printf("\n=== MENU ===\n");
        printf("1 - Check if a number is Prime\n");
        printf("2 - List all Primes below a limit (New Requirement)\n");
        printf("Type 'exit' to quit\n");
        printf("Select option: ");
        
        scanf("%1023s", inputString);


        if (strcmp(inputString, "exit") == 0) {
            break;
        }

        if (inputString[1] == '\0') { 
            switch(inputString[0]){
                case '1':
                    printf("Enter a large integer to check: ");
                    scanf("%1023s", inputString); 
       
                    if (mpz_set_str(n, inputString, 10) == -1) {
                        printf("Error: That is not a valid number.\n");
                        break;
                    }

                    int status = mpz_probab_prime_p(n, 25);

                    if (status == 2) {
                        printf("Result: The number is definitely prime.\n");
                    } else if (status == 1) {
                        printf("Result: The number is probably prime.\n");
                    } else {
                        printf("Result: The number is not definitely prime (Composite).\n");
                    }
                    break;

                case '2':
                    printf("Enter the Maximum Limit: ");
                    scanf("%1023s", inputString);

                    if (mpz_set_str(limit, inputString, 10) == -1) {
                        printf("Error: That is not a valid number.\n");
                        break;
                    }

                    printf("Primes below %s:\n", inputString);

                    mpz_set_ui(current, 2); 

                    while (mpz_cmp(current, limit) < 0) {
                        gmp_printf("%Zd ", current);
                        
                        mpz_nextprime(current, current);
                    }
                    printf("\n");
                    break;

                default:
                    printf("Invalid option. Please try again.\n");
            }
        } else {
            printf("Invalid command.\n");
        }
    }

    mpz_clears(n, limit, current, NULL);
    printf("Exiting program...\n");
    return 0;
}