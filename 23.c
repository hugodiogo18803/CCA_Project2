#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função para Cifragem/Decifragem Simétrica (XOR)
// No XOR simétrico, a mesma operação cifra e decifra.
void processar_simetrico() {
    mpz_t msg, chave, resultado;
    char filename[256], outname[256], keyStr[1024];
    clock_t start, end;

    mpz_inits(msg, chave, resultado, NULL);

    printf("\n--- Cifragem Simetrica (XOR) ---\n");
    printf("Nome do ficheiro: ");
    scanf("%255s", filename);

    printf("Introduza a Chave Simetrica (Numero): ");
    scanf("%1023s", keyStr);
    
    if (mpz_set_str(chave, keyStr, 10) == -1) {
        printf("Erro: Chave invalida.\n");
        return;
    }

    FILE *f_in = fopen(filename, "r");
    if (!f_in) { printf("Erro ao abrir ficheiro.\n"); return; }
    
    sprintf(outname, "%s.sim", filename);
    FILE *f_out = fopen(outname, "w");

    if (gmp_scanf("%Zd", msg) == 1) {
        start = clock();
        
        // Operação Simétrica: Resultado = Mensagem XOR Chave
        mpz_xor(resultado, msg, chave); 
        
        end = clock();

        gmp_fprintf(f_out, "%Zd", resultado);
        printf("[OK] Ficheiro processado como '%s'.\n", outname);
        printf("Tempo de execucao: %f segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
    } else {
        printf("Erro: O ficheiro deve conter um numero.\n");
    }

    fclose(f_in); fclose(f_out);
    mpz_clears(msg, chave, resultado, NULL);
}

// ... manter funções cifrar_ficheiro e decifrar_ficheiro do programa 21 ...

int main() {
    int opcao;
    do {
        printf("\n=== Exercicio 23: RSA + Simetrico ===\n");
        printf("1. Cifrar RSA (Assimetrico)\n");
        printf("2. Decifrar RSA (Assimetrico)\n");
        printf("3. Cifrar/Decifrar com Chave Simetrica (XOR)\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch(opcao) {
            case 1: /* chamada para cifrar_ficheiro_rsa */ break;
            case 2: /* chamada para decifrar_ficheiro_rsa */ break;
                // Note: No seu código final, integre as funções anteriores aqui
            case 3: processar_simetrico(); break;
        }
    } while (opcao != 0);

    return 0;
}