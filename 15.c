#include <stdio.h>
#include <gmp.h>    
#include <time.h>   
#include <stdlib.h>

/**
 * Função para gerar um número provavelmente primo.
 * @param p Variável de destino do primo.
 * @param bits Tamanho desejado em bits.
 * @param state Estado do gerador de números aleatórios.
 */
void gerar_primo(mpz_t p, unsigned long bits, gmp_randstate_t state)
{
    mpz_t rand_num;
    mpz_init(rand_num); // Inicializa variável temporária

    // Gera um número aleatório com o número de bits especificado
    mpz_urandomb(rand_num, state, bits);

    // Garante que o número seja ímpar (bit 0) e tenha a magnitude correta (bit bits-1)
    mpz_setbit(rand_num, 0);
    mpz_setbit(rand_num, bits - 1);

    // Encontra o próximo primo imediatamente superior ao número gerado
    mpz_nextprime(p, rand_num);

    mpz_clear(rand_num); // Liberta a memória temporária
}

int main()
{
    // Declaração das variáveis RSA (p, q, n, phi, e, d)
    mpz_t p, q, n, phi, e, d, p_minus_1, q_minus_1;
    unsigned long key_size_bits;
    
    // Variáveis para medição de CPU ticks
    clock_t inicio, fim;
    double tempo_primos, tempo_total;

    // Configuração do estado do gerador de números aleatórios
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL)); // Semente baseada no tempo atual

    // Inicialização das variáveis GMP
    mpz_inits(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);

    printf("=== Exercicio 14 (Modificado): RSA com Medicao de Tempo ===\n");
    printf("Introduza a dimensao da chave em bits (ex: 2048, 4096): ");
    if (scanf("%lu", &key_size_bits) != 1)
        return 1;

    // --- Início da medição de tempo global ---
    clock_t tempo_inicio_global = clock();

    // 1. Geração dos fatores primos p e q
    printf("\n[1/5] A gerar primos p e q...\n");
    inicio = clock(); 
    
    gerar_primo(p, key_size_bits / 2, state);
    gerar_primo(q, key_size_bits / 2, state);

    // Garante que p e q sejam diferentes
    while (mpz_cmp(p, q) == 0)
    {
        gerar_primo(q, key_size_bits / 2, state);
    }
    
    fim = clock();
    tempo_primos = (double)(fim - inicio) / CLOCKS_PER_SEC; // Conversão para segundos

    // 2. Cálculo do Módulo RSA (n = p * q)
    printf("[2/5] A calcular Modulo n...\n");
    mpz_mul(n, p, q);

    // 3. Cálculo da função Totiente de Euler: phi(n) = (p-1)*(q-1)
    printf("[3/5] A calcular Totiente phi(n)...\n");
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    // 4. Definição do expoente público 'e'
    printf("[4/5] A definir expoente publico e...\n");
    mpz_set_ui(e, 65537); // Valor comum por ser primo de Fermat
    mpz_t gcd_res;
    mpz_init(gcd_res);
    while (1)
    {
        // Garante que 'e' e 'phi' sejam coprimos (mdc = 1)
        mpz_gcd(gcd_res, e, phi);
        if (mpz_cmp_ui(gcd_res, 1) == 0) break;
        mpz_add_ui(e, e, 2);
    }
    mpz_clear(gcd_res);

    // 5. Cálculo do expoente privado 'd' (Inverso Modular de e mod phi)
    printf("[5/5] A calcular expoente privado d (Inverso Modular)...\n");
    inicio = clock();
    
    if (mpz_invert(d, e, phi) == 0)
    {
        fprintf(stderr, "ERRO: Inverso modular nao existe.\n");
        return 1;
    }
    
    fim = clock();
    double tempo_d = (double)(fim - inicio) / CLOCKS_PER_SEC;

    clock_t tempo_fim_global = clock();
    tempo_total = (double)(tempo_fim_global - tempo_inicio_global) / CLOCKS_PER_SEC;

    // --- Apresentação dos Relatórios ---
    printf("\n--- Relatorio de Performance ---\n");
    printf("Tempo para gerar primos:   %.6f segundos\n", tempo_primos);
    printf("Tempo para calcular 'd':   %.6f segundos\n", tempo_d);
    printf("Tempo TOTAL de geracao:    %.6f segundos\n", tempo_total);

    // Exibe as chaves em formato decimal (Zd)
    printf("\n=== Chaves Geradas ===\n");
    gmp_printf("Publica (e): %Zd\n", e);
    gmp_printf("Privada (d): %Zd\n", d);
    gmp_printf("Modulo (n):  %Zd\n", n);

    // Limpeza final de memória
    mpz_clears(p, q, n, phi, e, d, p_minus_1, q_minus_1, NULL);
    gmp_randclear(state);

    return 0;
}

