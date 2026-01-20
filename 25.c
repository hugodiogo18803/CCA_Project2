#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para manter o estado do RC4
typedef struct {
    unsigned char S[256];
    int i, j;
} RC4_State;

// Fase 1: Key Scheduling Algorithm (KSA)
void rc4_init(RC4_State *state, unsigned char *key, int key_len) {
    int i, j = 0;
    for (i = 0; i < 256; i++)
        state->S[i] = i;

    for (i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % key_len]) % 256;
        unsigned char temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }
    state->i = 0;
    state->j = 0;
}

// Fase 2: Pseudo-Random Generation Algorithm (PRGA)
unsigned char rc4_output(RC4_State *state) {
    state->i = (state->i + 1) % 256;
    state->j = (state->j + state->S[state->i]) % 256;

    unsigned char temp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = temp;

    return state->S[(state->S[state->i] + state->S[state->j]) % 256];
}

void processar_rc4(unsigned char *data, int len, unsigned char *key, int key_len) {
    RC4_State state;
    rc4_init(&state, key, key_len);
    for (int k = 0; k < len; k++) {
        data[k] ^= rc4_output(&state);
    }
}

int main() {
    unsigned char key[256];
    int key_len;
    char buffer[4096];
    int opcao, modo;

    // Leitura da chave de um ficheiro de texto (conforme requisito)
    FILE *fkey = fopen("chave.txt", "r");
    if (!fkey) {
        printf("Erro: ficheiro 'chave.txt' nao encontrado.\n");
        return 1;
    }
    key_len = fread(key, 1, 256, fkey);
    fclose(fkey);

    while (1) {
        printf("\n--- RC4: Cifrar/Decifrar ---\n");
        printf("1. Introduzir Mensagem/Criptograma\n0. Sair\nOpcao: ");
        scanf("%d", &opcao);
        if (opcao == 0) break;

        printf("Tipo de entrada:\n1. Inline (Texto)\n2. Ficheiro de Texto\n3. Ficheiro Binario\nEscolha: ");
        scanf("%d", &modo);

        if (modo == 1) {
            printf("Introduza a mensagem: ");
            scanf("%s", buffer);
            int len = strlen(buffer);
            processar_rc4((unsigned char*)buffer, len, key, key_len);
            printf("Resultado (Hex): ");
            for(int k=0; k<len; k++) printf("%02X ", (unsigned char)buffer[k]);
            printf("\n");
        } 
        else if (modo == 2 || modo == 3) {
            char filename[256];
            printf("Nome do ficheiro: ");
            scanf("%s", filename);
            
            FILE *f = fopen(filename, modo == 2 ? "r" : "rb");
            if (!f) { printf("Erro ao abrir ficheiro.\n"); continue; }
            
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);

            unsigned char *content = malloc(fsize);
            fread(content, 1, fsize, f);
            fclose(f);

            processar_rc4(content, fsize, key, key_len);

            FILE *fout = fopen("resultado.rc4", "wb");
            fwrite(content, 1, fsize, fout);
            fclose(fout);
            free(content);
            printf("[OK] Processado para 'resultado.rc4'\n");
        }
    }
    return 0;
}