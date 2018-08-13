#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <cstdio> 

using namespace std;

bool prime(unsigned long long  n){ 
	for(unsigned long long i = 2; i <= sqrt(n); i++)
		if(n % i == 0)
			return false;
	return true;
}

int binpow (int a, int n) {
	if (n == 0)
		return 1;
	if (n % 2 == 1)
		return binpow (a, n-1) * a;
	else {
		int b = binpow (a, n/2);
		return b * b;
	}
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

long Shamir(FILE *fp, FILE *fe, FILE *fd)
{
	long p, da, ca, db, cb, x1, x2, x3, x4;
	char m;
	srand(time(0));
	p = rand();
	while ((prime(p) == false)) {
		p = rand();
	}
	
	ca = (rand() % p) + 1;
	while (euclide(ca, p - 1, NULL) != 1) {
		ca = (rand() % p) + 1;
	}
	euclide(ca, p - 1, &da);
	if (da < 0)
		da = p + da - 1;
		
	cb = (rand() % p) + 1;
	while (euclide(cb, p - 1, NULL) != 1) {
		cb = (rand() % p) + 1;
	}
	euclide(cb, p - 1, &db);
	if (db < 0)
		db = p + db - 1;
	fseek(fe, 0L, SEEK_SET);
	while ((m = fgetc(fp)) != EOF){
		x1 = modpow(m, ca, p);
		x2 = modpow(x1, cb, p);
		x3 = modpow(x2, da, p);
		x4 = modpow(x3, db, p);
		fputc(m, fd);
	}
}

long root (long p, long q){
	long g;
	do{
		g = (rand() % (p - 1)) + 1;
	} while ((modpow(g, q, p)) != 1);
	
	return g;
}

long el_g(FILE *fp, FILE *fe, FILE *fd)
{
	long p, g, ca, cb, da, db, k, r, e, q;
	char m;
	
	srand(time(0));
	do {
		q = (rand() % 1000000) + 1;
		p = 2 * q + 1;
	} while (prime(p) != true);
	
	g = root(p, q);
	ca = ((rand()) % (p - 2)) + 1;
	cb = ((rand()) % (p - 2)) + 1;
	da = modpow(g, ca, p);
	db = modpow(g, cb, p);
	
	fseek(fe, 0L, SEEK_SET);
	while ((m = fgetc(fp)) != EOF){
		k = ((rand()) % (p - 3)) + 1;
		r = modpow(g, k, p);
		e = ((int)m * modpow (db, k, p))%p;
		fprintf(fe, "%li %li\n",r,  e);
	}
	fseek(fe, 0L, SEEK_SET);
	
	while (!feof(fe)){
		fscanf(fe, "%li", &r);
		fscanf(fe, "%li", &e);
		
		m = (char)((e * modpow (r, p - 1 - cb, p)) % p);
		fputc(m, fd);
	}
}

long vernam(FILE *fp, FILE *fe, FILE *fd)
{
	
	long k;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	char ch;
	fseek(fp, 0L, SEEK_SET);
	srand(time(0));
	
	long key[size];
	for (int i = 0; i < size; i++){
		ch = fgetc(fp);
		k = rand() % 127;
		ch ^= k;
		fputc(ch, fe);
		key[i] = k;
	}
	fseek(fe, 0L, SEEK_SET);
	for (int i = 0; i < size; i++){
		ch = fgetc(fe);
		k = key[i];
		ch ^= k;
		fputc(ch, fd);
	}
}

long RSA(FILE *fp, FILE *fe, FILE *fd)
{
	
	long P, Q, f, d, c, e, N;
	char ch;
	srand(time(0));
	P = rand();
	fseek(fe, 0L, SEEK_SET);
	
	do {
		P = (rand() % 100000) + 1;
	} while (prime(P) != true);
	do {
		Q = (rand() % 10000) + 1;
	} while (prime(Q) != true);
	N = P * Q;
	f = (P - 1) * (Q - 1);
	do {
		d = (rand() % f) + 1;
	} while (euclide(f, d, &c) != 1);
	if (c < 0)
		c = f + c;
	while ((ch = fgetc(fp)) != EOF){
		e = modpow((int)ch, d, N);
		fprintf(fe, "%li\n", e);
	}
	fseek(fe, 0L, SEEK_SET);
	while (!feof(fe)){
		fscanf(fe, "%li", &e);
		ch = modpow (e, c, N);
		fputc(ch, fd);
	}
}

using namespace std;
int main()
{
	FILE *fp, *fd, *fe;
	fp = fopen("message.in", "r");
	fe = fopen("encrypt.out", "w+");
	fd = fopen("decrypt.out", "w");
	Shamir(fp, fe, fd);
	//el_g(fp, fe, fd);
	//long x;
	//vernam(fp, fe, fd);
	//RSA(fp, fe, fd);
	//x = euclide(7, 11);
	//cout << x << endl;
	fclose(fp);
	fclose(fe);
	fclose(fd);
	return 0;
}
