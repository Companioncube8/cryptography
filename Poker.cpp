#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <climits>

using namespace std;

#define USERS 5
#define N_CARDS 52

string capture_list[4] = {"Spades" ,"Hearts", "Clubs", "Diamonds"};
string name_list[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};

struct st_players {
    long cards[2];
    long index[2];
    long crypt;
    long decrypt;
};

struct st_cards {
    long id;
    bool free;
};

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

void shuffle(st_cards *cards)
{
    int index;
    long swap_id;
    bool swap_free;
    for (int i = 0; i < N_CARDS; i++) {
        index = (int) rand() % N_CARDS;
        swap_id = cards[i].id;
        swap_free = cards[i].free;
        cards[i].id = cards[index].id;
        cards[i].free = cards[index].free;
        cards[index].id = swap_id;
        cards[index].free = swap_free;
    }
}

long root (long p, long q){
	long g;
	do{
		g = (rand() % (p - 1)) + 1;
	} while ((modpow(g, q, p)) != 1);
	
	return g;
}

int main()
{
	long p, d, c, r;
	st_players *players = new st_players[USERS];
	st_cards *card = new st_cards[N_CARDS], *deck = new st_cards[N_CARDS];

	srand(time(0));

	do {
		p = rand()% 100000;
	} while (prime(p) != true);
	
	for (int i = 0; i < USERS; i++){
		do {
			c = (rand() % (p - 2)) + 1;
		} while (euclide(c, (p - 1), &d) != 1);
		if (d < 0)
			d = p - 1 + d;
		players[i].crypt = c;
		players[i].decrypt = d;
	}
	for (int i = 0; i < N_CARDS; i++) {
		deck[i].id = rand() % (p - 2) + 1;
		deck[i].free = true;
		card[i].id = deck[i].id;
		card[i].free = true;
	} 
	for (int j = 0; j < USERS; j++) {
		for (int i = 0; i < N_CARDS; i++) {
			card[i].id = modpow(card[i].id, players[j].crypt, p);
		}
		shuffle(card);
	}
	
	for (int k = 0; k < 2; k++){
		for (int j = 0; j < USERS; j++) {
			do {
				r = rand() % N_CARDS;
			} while (card[r].free != true);
			card[r].free = false;
			
			players[j].cards[k] = card[r].id;
			
			for (int i = 0; i < USERS; i++) {
					if (i == j) 
						continue;
					players[j].cards[k] = modpow(players[j].cards[k], players[i].decrypt, p);
			}
			players[j].cards[k] = modpow(players[j].cards[k], players[j].decrypt, p);
		}
	}
	for (int i = 0; i < USERS; ++i) {
		cout << "PLAYER" << i <<": ";
		long l = 0;
		for (int k = 0; k < N_CARDS; k++) {
			if (players[i].cards[0] == deck[k].id || players[i].cards[1] == deck[k].id){
				cout << capture_list[k/13] << " " << name_list[k%13];
				deck[k].free = false;
				l++;
				if (l == 2)
					break;
				else 
					cout << ", ";
			}
		}
		cout << "\n";
	}
	
	cout << "TABLE CARD: ";
	for (int i = 0; i < 5; ++i) {
		do{
			r = rand() % N_CARDS;
		} while (deck[r].free != true);
		cout << capture_list[r / 13] << " " << name_list[r % 13];
		if (i != 4)
			cout << ", ";
	}
	cout << "\n";
	return 0;
}
