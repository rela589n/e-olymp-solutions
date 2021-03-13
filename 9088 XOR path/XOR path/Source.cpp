#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int st;

#define ALPHABET_SIZE 64
#define INFF LLONG_MAX

st n;
ll matrix[20][20];

struct Node {
	Node* ofBit[2] = {NULL, NULL};
};

Node* numbersXors[20];

ll minXors[20];

queue<tuple<st, st, ll>> toVisit;

void insertValue(Node* head, ll val) {

	auto cur = head;

	for (ll i = ALPHABET_SIZE - 1; i >= 0; --i) {
		bool bit = val & (1ll << i);

		if (cur->ofBit[bit] == NULL) {
			cur->ofBit[bit] = new Node();
		}

		cur = cur->ofBit[bit];
	}
}

void cleanup(Node*& head) {

	if (head->ofBit[0] != NULL) {
		cleanup(head->ofBit[0]);
	}

	if (head->ofBit[1] != NULL) {
		cleanup(head->ofBit[1]);
	}

	delete head;
}

void checkout_min(st j, ll xo) {
	Node* cur = numbersXors[j];

	ll res = 0;

	for (ll i = ALPHABET_SIZE - 1; i >= 0; --i) {
		bool bit = xo & (1ll << i);

		if (cur->ofBit[bit] != NULL) {
			cur = cur->ofBit[bit];
		}
		else {
			res |= (1ll << i);
			cur = cur->ofBit[!bit];
		}
	}

	if (res < minXors[j]) {
		minXors[j] = res;
	}
}

void evaluate(st from) {

	while (!toVisit.empty())
	{
		auto point = toVisit.front();
		toVisit.pop();

		st i = get<0>(point);
		st j = get<1>(point);
		ll xo = get<2>(point);

		xo ^= matrix[i][j];

		if (i == n - 1 && j == n - 1) {
			xo ^= matrix[n - from - 1][from];
			checkout_min(from, xo);
		}
		else {
			if (i != n - 1) {
				toVisit.push(make_tuple(i + 1, j, xo));
			}

			if (j != n - 1) {
				toVisit.push(make_tuple(i, j + 1, xo));
			}
		}
	}
}

void bruteInsert(Node* head, st from) 
{
	while (!toVisit.empty())
	{
		auto point = toVisit.front();
		toVisit.pop();

		st i = get<0>(point);
		st j = get<1>(point);
		ll xo = get<2>(point); // previous xor

		xo ^= matrix[i][j]; // overwrite with current one

		// if xor chain is complete
		if (i == 0 && j == 0) {
			// push current xor value into tree
			insertValue(numbersXors[from], xo);
		}
		else {
			// if can move upper
			if (i != 0) {
				toVisit.push(make_tuple(i - 1, j, xo));
			}
			
			// if can move to the left
			if (j != 0) {
				toVisit.push(make_tuple(i, j - 1, xo));
			}
		}
	}
}

int main() {
	cin >> n;

	for (st i = 0; i < n; ++i) {
		numbersXors[i] = new Node(); // create tree objects
		minXors[i] = INFF; // set value which is greater than any possible xor

		for (st j = 0; j < n; ++j) {
			cin >> matrix[i][j];
		}
	}

	toVisit = queue<tuple<st, st, ll>>();
	
	for (st j = 0; j < n; ++j) {
		// process top left

		toVisit.push(make_tuple(n - j - 1, j, 0));

		bruteInsert(numbersXors[j], j);

		// process bottom right

		toVisit.push(make_tuple(n - j - 1, j, 0));

		evaluate(j);

		cleanup(numbersXors[j]);
	}

	cout << *min_element(minXors, minXors + n);

	return 0;
}

/*
// 1
3
1 2 3
4 5 6
7 8 9

result values
{3, 1, 15}

trees
0      0       0
|      |       |
0      0       0
|     / \      |
0    0   1     0
|    |   |     |
1    0   1     0
|    |   |     |
0    0   0     0


// 241
4
99 146 613 1416
513 5810 1515 9616
1247 5124 6284 5844
1139 6135 6427 1561

*/