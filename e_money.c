#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include "md5.h"

long N, c[9], d[9];
int nomin[9] = {1, 2, 5, 10, 50, 100, 500, 1000, 5000};
int wallet = 10000;

int prime(unsigned long long  n){ 
	for(unsigned long long i = 2; i <= sqrt(n); i++)
		if(n % i == 0)
			return 0;
	return 1;
}


int modpow(unsigned long long base,unsigned long long exp,unsigned long long modulo){
	int r = 1;
	base = (base % modulo);
	while(exp > 0){
		int x = exp % 2;
		if(x > 0){
			r = (r * base) % modulo;
		}
		exp = exp / 2;
		base = (base * base) % modulo;
	}
	return r;
}

long root (long p, long q){
	long g;
	do{
		g = (rand() % (p - 1)) + 1;
	} while ((modpow(g, q, p)) != 1);
	
	return g;
}


long euclide(long a, long b, long *x)
{
    if (b > a)
    {
        int temp;
        temp = b;
        b = a;
        a = temp;
    }
    long U[3], V[3], q, T[3];
	U[0] = a;
	U[1] = 1;
	U[2] = 0;
	V[0] = b;
	V[1] = 0;
	V[2] = 1;
	while (V[0] != 0){
		q = U[0] / V[0];
		T[0] = U[0] % V[0];
		T[1] = U[1] - q*V[1];
		T[2] = U[2] - q*V[2];
		for (int i = 0; i < 3; i++){
			U[i] = V[i];
			V[i] = T[i];
		}
	} 
	if (x != NULL)
		*x = U[2];
    return U[0];
}

long bank_init()
{
	int i;
	long P, Q, f;
	do {
		P = (rand() % 100000) + 1;
	} while (prime(P) != 1);
	do {
		Q = (rand() % 10000) + 1;
	} while (prime(Q) != 1);
	N = P * Q;
	f = (P - 1) * (Q - 1);
	for (i = 0; i < 9; i++){
		do {
			d[i] = (rand() % f) + 1;
		} while (euclide(f, d[i], &c[i]) != 1);
		if (c[i] < 0)
			c[i] = f + c[i];
	}	
}

long bank_sign(long n, int i)
{
	wallet = wallet - nomin[i];
	return modpow(n, c[i], N);
}

long bank_check(long n, long s, int i)
{
	long h;
	MD5_CTX mdContext;
	MD5Init (&mdContext);
	MD5Update (&mdContext, (unsigned char*)&n, sizeof(long));
	MD5Final (&mdContext);
	h = 0;
    for (int k = 0; k < 16; k++) {
		h += mdContext.digest[k];
	}
	if (h == modpow(s, d[i], N)) {
                return 0;
    }
	return 1;
}

int store (long n, long s, int i)
{
	if (!bank_check(n, s, i)) {
        printf("Transaction is completed successfully (%ld %ld)\n", n, s);
        return 0;
    } else {
        printf("Transaction failed (%ld %ld)\n", n, s);
	}
}

long bayer()
{
	int money, bills[9]={0};
	int i, j, k;
	long n, h, s, s_, n_, r, r_;
	srand(time(0));
	printf("How much money will you spend? ");
	scanf("%d", &money);
	printf ("\n");
	while (money > wallet){
		printf("Insufficient funds on the account\n");
		printf("How much money will you spend? ");
		scanf("%d", &money);
		printf ("\n");
	}
	for (i = 8; money != 0; i--) {
        bills[i] = 0;
        bills[i] = money / nomin[i];
        if (bills[i]) {
            money -= bills[i] * nomin[i];
        }
	}
	printf("bills:\n");
    for (i = 0; i < 9; i++) {
        if (bills[i] != 0)
            printf("%d$ - %d\n", nomin[i], bills[i]);
	}
	for (i = 8; i >= 0; i--) {
		for (j = 0; j < bills[i]; j++) {
			n = (rand() % (N - 2)) + 2;
			MD5_CTX mdContext;
			MD5Init (&mdContext);
			MD5Update (&mdContext, (unsigned char*)&n, sizeof(long));
			MD5Final (&mdContext);
			h = 0;
            for (int k = 0; k < 16; k++) {
                h += mdContext.digest[k];
			}
			do {
				r = (rand() % N) + 1;
			} while (euclide(r, N, &r_) != 1);
			if (r_ < 0)
				r_ = N + r_;
			n_ = (h * modpow(r, d[i], N)) % N;
			s_ = bank_sign(n_, i);
			s = (s_ * r_)% N;
			store(n, s, i);
		}
	}
	printf("On the account of %d$ dollars\n", wallet);
}

int main()
{
	bank_init();
	bayer();
}
