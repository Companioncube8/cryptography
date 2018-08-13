#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <map>


using namespace std;

int colors[3];

struct edge
{
	int first;
	int second;
};

struct vert 
{
	long r;
	int color;
	long P, Q, f, N, d, c;
	long Z;
};

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

int prime(unsigned long long  n){ 
	for(unsigned long long i = 2; i <= sqrt(n); i++)
		if(n % i == 0)
			return false;
	return true;
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

void shuffle()
{
    int index, temp;
    for (int i = 0; i < 3; i++) {
        index = (int) rand() % 3;
        temp = colors[i];
        colors[i] = colors[index];
        colors[index] = temp;
    }
}

int main()
{
	struct edge *edges;
	struct vert *verts;
	long V, E;
	int E_check, a = 1;
	
	srand(time(0));
	FILE* file = fopen("graf.txt", "r");
	fscanf(file, "%ld %ld", &V, &E);
	edges = new struct edge[E];
	verts = new struct vert[V];
	
	colors [0]= 0;
	colors [1]= 1;
	colors [2]= 2;
	shuffle();
	
	for (int i = 0; i < E; i++) {
        fscanf(file, "%d %d", &edges[i].first ,&edges[i].second);
        edges[i].first--;
		edges[i].second--;
	}
	for (int i = 0; i < V; i++){
		int index;
		verts[i].r = (rand() % 100000) + 1;
		fscanf(file, "%d", &index);
		verts[i].r = ((verts[i].r >> 2) << 2) + colors[index];

		do {
			verts[i].P = (rand() % 100000) + 1;
		} while (prime(verts[i].P) != 1);
		do {
			verts[i].Q = (rand() % 10000) + 1;
		} while (prime(verts[i].Q) != 1);
		verts[i].N = verts[i].P * verts[i].Q;
		verts[i].f = (verts[i].P - 1) * (verts[i].Q - 1);
		do {
			verts[i].d = (rand() % verts[i].f) + 1;
		} while (euclide(verts[i].f, verts[i].d, &verts[i].c) != 1);
		if (verts[i].c < 0)
			verts[i].c = verts[i].f + verts[i].c;
		verts[i].Z = modpow(verts[i].r, verts[i].d, verts[i].N); 
	}

	for (int i = 0; i < a * E; i++){
		long Z1, Z2;
		E_check = rand() % E;
		Z1 = modpow(verts[edges[E_check].first].Z, verts[edges[E_check].first].c, verts[edges[E_check].first].N);
		Z2 = modpow(verts[edges[E_check].second].Z, verts[edges[E_check].second].c, verts[edges[E_check].second].N);
		Z1 &= 3;
		Z2 &= 3;
		if (Z1 == Z2){
			printf("Edge %d check failed\n", E_check);
			return 0;
		}
	}
	printf("All edge check success\n");
	return 0;
}

