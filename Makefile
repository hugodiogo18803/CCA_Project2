CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lgmp

PROGS = 1 3 5 7 9 11 13 15 17 19 21 23 25 \
        ex2_gcd ex4_powm ex6_time_pow2 \
        ex8_primes_arg ex10_primes_file ex12_factorization \
        ex16_rsa_secure ex18_rsa_storage ex22_rsa_suite \
        ex24_lcg main_menu

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(PROGS) *.o