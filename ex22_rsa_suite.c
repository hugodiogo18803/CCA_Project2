/*
 * Exercício 22 (inclui Ex 19): Suite RSA com Controlo de Primos e Cifra de Ficheiros
 * * Funcionalidades:
 * 1. Gerar Chaves com restrição rigorosa de bits.
 * - Primos p, q devem estar entre 2^(k-1) e 2^k - 1.
 * 2. Carregar Chaves de ficheiros .myasc (Ex 19).
 * 3. Cifrar Ficheiro (Ex 19).
 * 4. Decifrar Ficheiro (Ex 19).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h>

// Leitura segura de entropia
unsigned long get_secure_seed()
{
    unsigned long seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if (!f)
        return time(NULL); // evitar crash
    fread(&seed, sizeof(seed), 1, f);
    fclose(f);
    return seed;
}

// Verifica se ficheiro existe
int file_exists(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

// EXERCÍCIO 22:
/*
 * Gera um primo estritamente dentro do intervalo [2^(bits-1), 2^bits - 1]
 * Isto garante que o primo tem EXATAMENTE 'bits' de comprimento.
 */
void gerar_primo_ex22(mpz_t p, unsigned long bits, gmp_randstate_t state)
{
    mpz_t min_limit, max_limit, rand_num;
    mpz_inits(min_limit, max_limit, rand_num, NULL);

    // Definir o limite inferior: 2^(bits-1)
    mpz_ui_pow_ui(min_limit, 2, bits - 1);

    // Gerar um número aleatório de 'bits' de comprimento
    // mpz_urandomb gera entre 0 e 2^bits - 1
    mpz_urandomb(rand_num, state, bits);

    // Forçar o bit mais significativo a 1.
    // Isto coloca o número automaticamente >= 2^(bits-1).
    mpz_setbit(rand_num, bits - 1);

    // Garantir ímpar
    mpz_setbit(rand_num, 0);

    // Encontrar o próximo primo
    mpz_nextprime(p, rand_num);

    // Se mpz_nextprime ultrapassar 2^bits (raro, mas possível se rand_num for max)
    if (mpz_sizeinbase(p, 2) > bits)
    {
        // Se ultrapassou, tenta novamente
        gerar_primo_ex22(p, bits, state);
    }

    mpz_clears(min_limit, max_limit, rand_num, NULL);
}

void gerar_chaves_menu()
{
    mpz_t p, q, n, phi, e, d, pm1, qm1, gcd_res;
    unsigned long key_bits;
    gmp_randstate_t state;

    gmp_randinit_default(state);
    gmp_randseed_ui(state, get_secure_seed());
    mpz_inits(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);

    printf("\n--- Gerar Chaves RSA ---\n");
    printf("Dimensao TOTAL da chave (bits, ex: 1024): ");
    if (scanf("%lu", &key_bits) != 1)
        return;

    // RSA requer dois primos de metade do tamanho
    unsigned long prime_bits = key_bits / 2;
    printf("Requisito Ex 22: Gerar primos estritamente com %lu bits...\n", prime_bits);

    gerar_primo_ex22(p, prime_bits, state);
    gerar_primo_ex22(q, prime_bits, state);
    while (mpz_cmp(p, q) == 0)
        gerar_primo_ex22(q, prime_bits, state);

    mpz_mul(n, p, q); // n
    mpz_sub_ui(pm1, p, 1);
    mpz_sub_ui(qm1, q, 1);
    mpz_mul(phi, pm1, qm1); // phi

    mpz_set_ui(e, 65537); // e
    while (1)
    {
        mpz_gcd(gcd_res, e, phi);
        if (mpz_cmp_ui(gcd_res, 1) == 0)
            break;
        mpz_add_ui(e, e, 2);
    }

    if (mpz_invert(d, e, phi) == 0)
    {
        printf("Erro na geracao. Tente novamente.\n");
        return;
    }

    // Guardar
    FILE *fp = fopen("public.myasc", "w");
    gmp_fprintf(fp, "%Zd,%Zd", e, n);
    fclose(fp);

    FILE *fs = fopen("secret.myasc", "w");
    gmp_fprintf(fs, "%Zd,%Zd\n\n%Zd,%Zd", e, n, d, n);
    fclose(fs);

    printf("Chaves geradas e guardadas em .myasc!\n");
    mpz_clears(p, q, n, phi, e, d, pm1, qm1, gcd_res, NULL);
    gmp_randclear(state);
}

// EXERCÍCIO 19:

void processar_ficheiro(int modo_cifra)
{
    char f_in[256], f_out[256];
    mpz_t key_part1, key_part2, n, m, c;
    mpz_inits(key_part1, key_part2, n, m, c, NULL);

    printf("\n--- %s ---\n", modo_cifra ? "Cifrar (RSA Encrypt)" : "Decifrar (RSA Decrypt)");

    // Ler Chaves
    FILE *fk = NULL;
    if (modo_cifra)
    {
        // Cifrar usa Chave Publica (e, n) -> public.myasc
        fk = fopen("public.myasc", "r");
        if (!fk)
        {
            printf("Erro: public.myasc nao encontrado.\n");
            return;
        }
        gmp_fscanf(fk, "%Zd,%Zd", key_part1, n); // Lê e, n
    }
    else
    {
        // Decifrar usa Chave Privada (d, n) -> secret.myasc
        fk = fopen("secret.myasc", "r");
        if (!fk)
        {
            printf("Erro: secret.myasc nao encontrado.\n");
            return;
        }
        // O formato é: publica \n \n privada. Precisamos saltar para a 3a linha
        gmp_fscanf(fk, "%*Zd,%*Zd");             // Ignora linha 1
        gmp_fscanf(fk, "%Zd,%Zd", key_part1, n); // Lê d, n da linha 3 (após o skip)
    }
    fclose(fk);

    // Pedir Ficheiros
    printf("Ficheiro de entrada: ");
    scanf("%s", f_in);
    if (!file_exists(f_in))
    {
        printf("Ficheiro nao existe.\n");
        return;
    }

    printf("Ficheiro de saida: ");
    scanf("%s", f_out);

    // Ler Entrada para Memória
    FILE *fin = fopen(f_in, "rb");

    // Obter tamanho do ficheiro
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    unsigned char *buffer = malloc(fsize);
    fread(buffer, 1, fsize, fin);
    fclose(fin);

    // Converter Bytes <-> MPZ
    if (modo_cifra)
    {
        // IMPORTAR: Bytes -> Numero Gigante
        mpz_import(m, fsize, 1, 1, 0, 0, buffer);

        // Validar Tamanho (Mensagem < Modulo)
        if (mpz_cmp(m, n) >= 0)
        {
            printf("ERRO: O ficheiro e demasiado grande para esta chave RSA!\n");
            printf("Aumente a chave ou use ficheiros menores.\n");
            free(buffer);
            return;
        }

        // CÁLCULO: c = m^e mod n
        mpz_powm(c, m, key_part1, n);

        // EXPORTAR PARA FICHEIRO
        FILE *fout = fopen(f_out, "w");
        gmp_fprintf(fout, "%Zd", c);
        fclose(fout);
    }
    else
    {
        // MODO DECIFRA
        // Ler o criptograma
        FILE *fin_c = fopen(f_in, "r");
        gmp_fscanf(fin_c, "%Zd", c);
        fclose(fin_c);

        // CÁLCULO: m = c^d mod n
        mpz_powm(m, c, key_part1, n);

        // EXPORTAR: Numero -> Bytes
        size_t count;
        // mpz_export(ptr, count_ptr, order, size, endian, nails, op)
        unsigned char *out_buf = mpz_export(NULL, &count, 1, 1, 0, 0, m);

        FILE *fout = fopen(f_out, "wb");
        fwrite(out_buf, 1, count, fout);
        fclose(fout);
        free(out_buf); // mpz_export aloca memória, temos de libertar
    }

    free(buffer);
    printf("Operacao concluida com sucesso.\n");
    mpz_clears(key_part1, key_part2, n, m, c, NULL);
}

// MAIN LOOP
int main()
{
    int opcao = -1;
    while (opcao != 0)
    {
        printf("\n=== Exercicio 22 ===\n");
        printf("1. Gerar Chaves \n");
        printf("2. Cifrar Ficheiro (RSA)\n");
        printf("3. Decifrar Ficheiro (RSA)\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ; // Limpar buffer
            continue;
        }

        switch (opcao)
        {
        case 1:
            gerar_chaves_menu();
            break;
        case 2:
            processar_ficheiro(1);
            break;
        case 3:
            processar_ficheiro(0);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    }
    return 0;
}