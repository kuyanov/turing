#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <array>
#include <cassert>
#define x first
#define y second
using namespace std;

map<pair<int, int>, vector<int> > machine;

void simulate(string start)
{
	int pos = 1, state = 0;
	for (long long it = 0; it < 1e16; it++)
	{
		if (it % 1 == 0)
		{
			cout << start << '\n';
			string pos_str(start.size(), ' ');
			pos_str[pos] = '^';
			cout << pos_str << '\n' << state << '\n';
		}
		int cur = start[pos] - '0';
		if (machine.find({ state, cur }) == machine.end())
		{
			cout << "FINISH " << it << '\n';
			return;
		}
		auto elem = machine[{state, cur}];
		start[pos] = elem[0] + '0';
		pos += elem[1];
		if (start.size() <= pos)
			start += '0';
		state = elem[2];
	}
}

void read()
{
	int n, a, b, c, d, e;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> a >> b >> c >> d >> e;
		machine[{a, b}] = {c, d, e};
	}
}

vector<pair<string, string>> mpcp, pcp;

void build_MPCP_instance(int final_state)
{
	assert(final_state < 26);
	string abc = "abcdefghijklmnopqrstuvwxyz", sm = "#012";
	mpcp.emplace_back("#", "#2a0002#");
	for (int i = 0; i < sm.length(); ++i)
		mpcp.emplace_back(sm.substr(i, 1), sm.substr(i, 1));
	for (auto rule : machine)
	{
		if (rule.y[1] == 1)
		{
			mpcp.emplace_back(abc.substr(rule.x.x, 1) + char(rule.x.y + '0'), string() + char(rule.y[0] + '0') + abc[rule.y[2]]);
			if (!rule.x.y)
				mpcp.emplace_back(abc.substr(rule.x.x, 1) + '#', string() + char(rule.y[0] + '0') + abc[rule.y[2]] + '#');
		}
		else if (rule.y[0])
		{
			for (int i = 1; i < sm.size(); ++i)
			{
				mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + char(rule.x.y + '0'), abc.substr(rule.y[2], 1) + sm[i] + char(rule.y[0] + '0'));
				if (!rule.x.y)
					mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + '#', abc.substr(rule.y[2], 1) + sm[i] + char(rule.y[0] + '0') + '#');
			}
		}
		else if (rule.x.y)
		{
			for (int i = 1; i < sm.size(); ++i)
			{
				for (int j = 1; j < sm.size(); ++j)
				{
					mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + char(rule.x.y + '0') + sm[j], abc.substr(rule.y[2], 1) + sm[i] + '0' + sm[j]);
				}
				mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + char(rule.x.y + '0') + '#', abc.substr(rule.y[2], 1) + (i > 1 ? sm.substr(i, 1) : "") + '#');
			}
		}
		else
		{
			for (int i = 1; i < sm.size(); ++i)
			{
				mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + '0', abc.substr(rule.y[2], 1) + sm[i] + '0');
				mpcp.emplace_back(sm.substr(i, 1) + abc[rule.x.x] + '#', abc.substr(rule.y[2], 1) + (i > 1 ? sm.substr(i, 1) : "") + '#');
			}
		}
	}
	mpcp.emplace_back(abc.substr(final_state, 1) + "##", "#");
}

void build_PCP_instance() {
	for (int i = 0; i < (int)mpcp.size(); i++) {
		pcp.emplace_back("", "");
		for (char c: mpcp[i].first) {
			pcp.back().first += c;
			pcp.back().first += '$';
		}
		for (char c : mpcp[i].second) {
			pcp.back().second += '$';
			pcp.back().second += c;
		}
	}
	pcp[0].first = "$" + pcp[0].first;
	pcp.back().second += '$';
}

int main()
{
	read();
	cout << "ready" << endl;
	//simulate("2002");
	int final_state;
	//cin >> final_state;
	final_state = 19;
	cout << '[' << final_state << ']';
	build_MPCP_instance(final_state);
	build_PCP_instance();
	cout << pcp.size() << '\n';
	for (auto f : pcp)
		cout << f.x << ' ' << f.y << '\n';
	return 0;
}
