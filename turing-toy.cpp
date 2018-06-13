#include <bits/stdc++.h>
using namespace std;

struct prelaz {
	string a, s, a2, s2;
	bool levo;
};

void ucitaj_set(set<string>& Y) {
	string s;
	getline(cin, s);
	istringstream iss(s);
	while (iss >> s)
		Y.insert(s);
}

void ucitaj_vec(vector<string>& Y) {
	string s;
	getline(cin, s);
	istringstream iss(s);
	while (iss >> s)
		Y.push_back(s);
}

void stampaj_traku(const deque<string>& traka, int i) {
	int j = 0;
	for (auto s : traka) {
		cerr << s << ' ';
		if (i > 0) {
			j += (int)s.size() + 1;
			i--;
		}
	}
	cerr << "\n" << string(j, ' ') << "^\n";
}

pair<int, vector<string>> pokreni(
	const set<string>& X,
	const set<string>& Sigma,
	const set<string>& A,
	const set<string>& T,
	const string& a0,
	const vector<prelaz>& delta,
	vector<string> traka_)
{

	deque<string> traka(traka_.begin(), traka_.end());

	int steps = 0;
	int i = 0;
	string a = a0;
	while (steps < 10000000) {
		steps++;
		stampaj_traku(traka, i);

		bool found = false;
		for (auto p : delta) {
			if (traka[i] == p.s && a == p.a) {
				found = true;
				traka[i] = p.s2;
				a = p.a2;
				if (p.levo) {
					i--;
					if (i == -1) {
						i = 0;
						traka.push_front("#");
					}
				} else {
					i++;
					if (i == (int)traka.size())
						traka.push_back("#");
				}
				break;
			}
		}
		if (!found)
			break;
	}

	traka_ = vector<string>(traka.begin(), traka.end());

	if (steps == 10000000)
		return {-1, {}};
	return {2*steps + (1 - T.count(a)), traka_};
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cerr.tie(nullptr);

	set<string> X, Sigma, A, T;
	string a0;
	vector<prelaz> delta;
	vector<string> traka;

	cerr << "Default (prazni) simbol je #.\n";
	cerr << "Simboli (u jednom redu, odvojeni razmakom):\n" << flush;
	Sigma = X;
	ucitaj_set(Sigma);

	cerr << "Stanja automata (u jednom redu, odvojeni razmakom):\n" << flush;
	ucitaj_set(A);

	cerr << "Pocetno stanje: " << flush;
	getline(cin, a0);

	cerr << "Zavrsna stanja (u jednom redu, odvojeni razmakom):\n" << flush;
	ucitaj_set(T);

	cerr << "Prelazi (svaki u svom redu, prazan red je kraj)\n" << flush;
	cerr << "Format: <stanje> <simbol> <novo stanje> <novi simbol> <smer>\n" << flush;
	while (1) {
		string s;
		getline(cin, s);
		if (s == "")
			break;
		istringstream iss(s);
		string a, b, c, d, e;
		iss >> a >> b >> c >> d >> e;
		delta.push_back({a, b, c, d, e == "L"});
	}

	cerr << "Pocetno stanje trake (u jednom redu, odvojeni razmakom):\n" << flush;
	ucitaj_vec(traka);

	cerr << traka.size() << '\n';

	pair<int, vector<string>> rez = pokreni(X, Sigma, A, T, a0, delta, traka);
	if (rez.first >= 0) {
		if (rez.first % 2)
			cerr << "Rec odbijena, broj koraka = " << rez.first / 2 << '\n';
		else
			cerr << "Rec prihvacena, broj koraka = " << rez.first / 2 << '\n';

		cerr << "Izgled trake:\n";
		for (string s : rez.second)
			cerr << s << ' ';
		cerr << '\n';
	} else if (rez.first == -1)
		cerr << "Prekoracen broj koraka\n";
	else if (rez.first == -2)
		cerr << "Izleteo sa trake sa leve strane\n"; // kao da se zaustavila
	else
		cerr << "Nesto nepredvidjeno se desilo\n";
}