#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>

void cifrar_ficheiro() {
    mpz_t e, n, m, c;
    char filename[256], outname[256];
    mpz_inits(e, n, m, c, NULL);

    // 1. Ler Chave Pública
    FILE *f_pub = fopen("public.myasc", "r");
    if (!f_pub) { printf("Erro: public.myasc não encontrado.\n"); return; }
    gmp_fscanf(f_pub, "%Zd,%Zd", e, n);
    fclose(f_pub);

    printf("Nome do ficheiro para CIFRAR: ");
    scanf("%255s", filename);

    // 2. Processar Ficheiros
    FILE *f_in = fopen(filename, "r");
    if (!f_in) { printf("Erro ao abrir ficheiro de entrada.\n"); return; }
    
    sprintf(outname, "%s.cifra", filename);
    FILE *f_out = fopen(outname, "w");

    // Lógica simplificada: Cifra o valor numérico contido no ficheiro
    if (gmp_fscanf(f_in, "%Zd", m) == 1) {
        mpz_powm(c, m, e, n); // Operação RSA: C = M^e mod n
        gmp_fprintf(f_out, "%Zd", c);
        printf("[OK] Ficheiro cifrado como '%s'.\n", outname);
    } else {
        printf("Erro: O ficheiro deve conter um número (mensagem).\n");
    }

    fclose(f_in); fclose(f_out);
    mpz_clears(e, n, m, c, NULL);
}

void decifrar_ficheiro() {
    mpz_t d, n, c, m;
    char filename[256], outname[256];
    mpz_inits(d, n, c, m, NULL);

    // 1. Ler Chave Privada (conforme formato do Ex 18)
    FILE *f_sec = fopen("secret.myasc", "r");
    if (!f_sec) { printf("Erro: secret.myasc não encontrado.\n"); return; }
    
    // O Ex 18 guarda a privada na 3ª linha (pula pública e linha vazia)
    char buffer[2048];
    fgets(buffer, sizeof(buffer), f_sec); // Linha 1 (e,n)
    fgets(buffer, sizeof(buffer), f_sec); // Linha 2 (vazia)
    gmp_fscanf(f_sec, "%Zd,%Zd", d, n);   // Linha 3 (d,n)
    fclose(f_sec);

    printf("Nome do ficheiro para DECIFRAR: ");
    scanf("%255s", filename);

    FILE *f_in = fopen(filename, "r");
    if (!f_in) { printf("Erro ao abrir ficheiro.\n"); return; }
    
    sprintf(outname, "%s.orig", filename);
    FILE *f_out = fopen(outname, "w");

    if (gmp_fscanf(f_in, "%Zd", c) == 1) {
        mpz_powm(m, c, d, n); // Operação RSA: M = C^d mod n
        gmp_fprintf(f_out, "%Zd", m);
        printf("[OK] Ficheiro decifrado como '%s'.\n", outname);
    }

    fclose(f_in); fclose(f_out);
    mpz_clears(d, n, c, m, NULL);
}

int main() {
    int opcao;
    do {
        printf("\n=== Exercicio 19: RSA File Crypt ===\n");
        printf("1. Cifrar Ficheiro (usa public.myasc)\n");
        printf("2. Decifrar Ficheiro (usa secret.myasc)\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cifrar_ficheiro(); break;
            case 2: decifrar_ficheiro(); break;
        }
    } while (opcao != 0);

    return 0;
}