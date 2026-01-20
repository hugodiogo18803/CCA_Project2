#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>

// Função para cifrar um ficheiro usando a chave pública
void cifrar_ficheiro()
{
    mpz_t e, n, m, c;
    char filename[256];
    // Aumentado para 270 para evitar avisos de overflow ao anexar ".cifra"
    char outname[270];

    // Inicialização das variáveis GMP
    mpz_inits(e, n, m, c, NULL);

    // 1. Leitura da Chave Pública (formato: e,n)
    FILE *f_pub = fopen("public.myasc", "r");
    if (!f_pub)
    {
        printf("Erro: ficheiro 'public.myasc' nao encontrado.\n");
        mpz_clears(e, n, m, c, NULL);
        return;
    }
    gmp_fscanf(f_pub, "%Zd,%Zd", e, n);
    fclose(f_pub);

    printf("Nome do ficheiro para CIFRAR: ");
    scanf("%255s", filename);

    // 2. Abertura do ficheiro de entrada
    FILE *f_in = fopen(filename, "r");
    if (!f_in)
    {
        printf("Erro ao abrir ficheiro de entrada.\n");
        mpz_clears(e, n, m, c, NULL);
        return;
    }

    // Criação do nome do ficheiro de saída
    sprintf(outname, "%s.cifra", filename);
    FILE *f_out = fopen(outname, "w");

    // Lógica: Lê o número do ficheiro, cifra-o e guarda o resultado
    if (gmp_fscanf(f_in, "%Zd", m) == 1)
    {
        // Operação RSA: C = M^e mod n
        mpz_powm(c, m, e, n);
        gmp_fprintf(f_out, "%Zd", c);
        printf("[OK] Ficheiro cifrado com sucesso como '%s'.\n", outname);
    }
    else
    {
        printf("Erro: O ficheiro de entrada deve conter um numero (mensagem).\n");
    }

    fclose(f_in);
    fclose(f_out);
    // Libertação de memória
    mpz_clears(e, n, m, c, NULL);
}

// Função para decifrar um ficheiro usando a chave privada
void decifrar_ficheiro()
{
    mpz_t d, n, c, m;
    char filename[256];
    // Aumentado para 270 para evitar avisos de overflow ao anexar ".orig"
    char outname[270];

    mpz_inits(d, n, c, m, NULL);

    // 1. Leitura da Chave Privada (conforme formato do Ex 18)
    FILE *f_sec = fopen("secret.myasc", "r");
    if (!f_sec)
    {
        printf("Erro: ficheiro 'secret.myasc' nao encontrado.\n");
        mpz_clears(d, n, c, m, NULL);
        return;
    }

    // Salta a chave pública e a linha vazia para ler a privada na 3ª linha
    char buffer[2048];
    fgets(buffer, sizeof(buffer), f_sec); // Linha 1: e,n
    fgets(buffer, sizeof(buffer), f_sec); // Linha 2: vazia
    gmp_fscanf(f_sec, "%Zd,%Zd", d, n);   // Linha 3: d,n
    fclose(f_sec);

    printf("Nome do ficheiro para DECIFRAR: ");
    scanf("%255s", filename);

    FILE *f_in = fopen(filename, "r");
    if (!f_in)
    {
        printf("Erro ao abrir ficheiro cifrado.\n");
        mpz_clears(d, n, c, m, NULL);
        return;
    }

    sprintf(outname, "%s.orig", filename);
    FILE *f_out = fopen(outname, "w");

    if (gmp_fscanf(f_in, "%Zd", c) == 1)
    {
        // Operação RSA: M = C^d mod n
        mpz_powm(m, c, d, n);
        gmp_fprintf(f_out, "%Zd", m);
        printf("[OK] Ficheiro decifrado com sucesso como '%s'.\n", outname);
    }
    else
    {
        printf("Erro: O ficheiro de entrada deve conter um criptograma valido.\n");
    }

    fclose(f_in);
    fclose(f_out);
    mpz_clears(d, n, c, m, NULL);
}

int main()
{
    int opcao;
    do
    {
        printf("\n=== Exercicio 19: RSA File Crypt ===\n");
        printf("1. Cifrar Ficheiro (usa public.myasc)\n");
        printf("2. Decifrar Ficheiro (usa secret.myasc)\n");
        printf("0. Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa o buffer em caso de erro de leitura
            opcao = -1;
        }

        switch (opcao)
        {
        case 1:
            cifrar_ficheiro();
            break;
        case 2:
            decifrar_ficheiro();
            break;
        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}