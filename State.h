#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
using namespace std; 

class State {
	string name;
	bool isFinal, isInitial;
	vector<string> points;
public:
	State(string n, int f, int i, vector<string> p) {
		name = n;
		isFinal = f;
		isInitial = i;
		points = p;
	}
	void setname(string n) {
		name = n;
	}
	string getname() {
		return name;
	}
	void setfinal() {
		isFinal = 1;
	}
	void setinitial() {
		isInitial = 1;
	}
	void setpoints(string s) {
		points.push_back(s);
	}
	void updatepoints(string p1, string p2) {
		for (auto it = points.begin(); it < points.end(); ++it) {
			string replace = *it;
			if (strstr(p1.c_str(), replace.c_str())) {
				*it = p1;
			}
		}
	}
	bool isfinal() {
		return isFinal;
	}
	bool isinitial() {
		return isInitial;
	}
	vector<string> getpoints() {
		return points;
	}
	bool operator == (const State& s) const {
		return this->name == s.name;
	}
	bool operator < (const State& s) const {
		return this->name < s.name;
	}
	friend ostream& operator<<(ostream& os, const State& st)
	{
		os << st.name << ', ';
		return os;
	}
};
