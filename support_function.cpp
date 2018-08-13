#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

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


bool prime(unsigned long long  n){ 
	for(unsigned long long i = 2; i <= sqrt(n); i++)
		if(n % i == 0)
			return false;
	return true;
}

long root (long p){
	long g;
	g = ((rand()) / (p - 1)) + 1;
	while ((modpow(g, (p - 1)/2, p)) == 1) 
		g = (rand() / (p - 1)) + 1;
	return g;
}

void dh(){
	unsigned long long g, q, p, Xa, Xb, Ya, Yb, Zab, Zba;
	srand(time(0));
	p = rand();
	while ((prime(p) == false) && (prime((p - 1)/2) == false)) {
		p = rand();
	}
	g = root(p);
	Xa = rand();
	Xb = rand();
	Ya = modpow(g, Xa, p);
	Yb = modpow(g, Xb, p);
	Zab = modpow(Yb, Xa, p);
	Zba = modpow(Ya, Xb, p);
	cout << "Zab = " << Zab << " Zba = " << Zba << endl;
}


long euclide(long a, long b, long *x, long *y)
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
	while (U[0] != 0){
		q = U[0] / V[0];
		T[0] = U[0] % V[0];
		T[1] = U[1] - q*V[1];
		T[2] = U[2] - q*V[3];
		*U = *V;
		*V = *T;
	} 
    x = &U[1];
    y = &U[2];
    return U[0];
}

long bsgs(unsigned long a, unsigned long long p, unsigned long y) 
{
	unsigned long k, m, j, i;
	k = (unsigned long)sqrt(p) + 1;
	m = k;
	map <unsigned long, unsigned long> mas;
	map <unsigned long, unsigned long>::iterator it;
	for (i = 0; i < m; i++){
		mas.insert(pair <unsigned long, unsigned long>(((unsigned long)pow((long double)a, (long double)i) * y) % p, i));
    }
    for (j = 1; j <= k; j++){
	    unsigned long temp = ((unsigned long)pow((long double)a, (long double)j * m)) % p;
        it = mas.find(temp);
        if (it != mas.end()){
            break;
        }
    }
	return (j * m) - it->second;
}

int main()
{
	//bool k = prime(113253456);
	//long x, y, g;
	//g = euclide (10, 15, &x, &y);
	dh();
	long d = bsgs(5, 23, 3);
	//cout << g << endl;
}
