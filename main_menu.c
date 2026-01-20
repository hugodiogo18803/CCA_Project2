#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Funções Auxiliares ---
void header(const char *titulo)
{
    system("clear");
    printf("========================================\n");
    printf("   MESTRADO ESI - CRIPTOGRAFIA (GMP)    \n");
    printf("========================================\n");
    printf(">> %s\n\n", titulo);
}

void pause_console()
{
    printf("\nPrima ENTER para voltar ao menu...");
    while (getchar() != '\n')
        ;
    getchar();
}

// Wrapper Genérico (para programas sem argv)
void run(const char *prog)
{
    char cmd[50];
    sprintf(cmd, "./%s", prog); // Executa ./1, ./3, ./ex2_gcd, etc.
    printf("A iniciar '%s'...\n", prog);
    system(cmd);
    pause_console();
}

// Wrapper para Ex 8 (Argumentos: Limite)
void run_ex8()
{
    char limit[100], cmd[200];
    header("Ex 8: Primos (Argv)");
    printf("Introduza o limite maximo: ");
    scanf("%s", limit);
    sprintf(cmd, "./ex8_primes_arg %s", limit);
    system(cmd);
    pause_console();
}

// Wrapper para Ex 10 (Argumentos: Limite + Ficheiro)
void run_ex10()
{
    char limit[100], filename[100], cmd[300];
    header("Ex 10: Primos para Ficheiro");
    printf("Introduza o limite maximo: ");
    scanf("%s", limit);
    printf("Introduza o nome do ficheiro de saida: ");
    scanf("%s", filename);
    sprintf(cmd, "./ex10_primes_file %s %s", limit, filename);
    system(cmd);
    pause_console();
}

int main()
{
    int opcao = -1;

    while (opcao != 0)
    {
        header("MENU GERAL (1-25)");

        // Coluna Esquerda (Ímpares - Básicos/Primos)
        printf(" 1. Multiplicacao (GMP)       |  2. GCD (MDC)\n");
        printf(" 3. Verificar Primo           |  4. Exponenciacao Modular\n");
        printf(" 5. Timing Exponenciacao      |  6. Timing Potencias de 2\n");
        printf(" 7. Listar Primos (Ciclo)     |  8. Listar Primos (Argv)\n");
        printf(" 9. Primos p/ Ficheiro (User) | 10. Primos p/ Ficheiro (Argv)\n");

        // Coluna Direita (Avançados/RSA)
        printf("11. Combinacoes               | 12. Fatorizacao de Pares\n");
        printf("13. Fatorizacao Formatada     | 14. [Use o 16 ou 22]\n");
        printf("15. Timing RSA KeyGen         | 16. RSA KeyGen Seguro\n");
        printf("17. Graficos Tempos RSA       | 18. RSA Storage (.myasc)\n");
        printf("19. RSA Cifra/Decifra         | 20. Analise Critica (Doc)\n");
        printf("21. RSA com Time Control      | 22. RSA Suite Completa\n");
        printf("23. RSA + Chave Simetrica     | 24. Gerador LCG\n");
        printf("25. Cifra RC4                 |  0. SAIR\n");

        printf("\nEscolha o Exercicio: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (opcao)
        {
        // --- ÍMPARES (1.c, 3.c ...) ---
        case 1:
            run("1");
            break;
        case 3:
            run("3");
            break;
        case 5:
            run("5");
            break;
        case 7:
            run("7");
            break;
        case 9:
            run("9");
            break;
        case 11:
            run("11");
            break;
        case 13:
            run("13");
            break;
        case 15:
            run("15");
            break;
        case 17:
            run("17");
            break;
        case 19:
            run("19");
            break;
        case 21:
            run("21");
            break;
        case 23:
            run("23");
            break;
        case 25:
            run("25");
            break;

        // --- PARES (Nomes descritivos anteriores) ---
        case 2:
            run("ex2_gcd");
            break;
        case 4:
            run("ex4_powm");
            break;
        case 6:
            run("ex6_time_pow2");
            break;
        case 8:
            run_ex8();
            break;
        case 10:
            run_ex10();
            break;
        case 12:
            run("ex12_factorization");
            break;
        // Nota: Ex 14 foi superado pelo 16/18/22, mas se tiveres "14.c", muda para run("14")
        case 16:
            run("ex16_rsa_secure");
            break;
        case 18:
            run("ex18_rsa_storage");
            break;
        case 22:
            run("ex22_rsa_suite");
            break;
        case 24:
            run("ex24_lcg");
            break;

        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opcao nao implementada ou invalida!\n");
            pause_console();
        }
    }
    return 0;
}