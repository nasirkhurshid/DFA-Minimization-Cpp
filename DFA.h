#include "State.h";

class DFA {
	int noOfSymbols, noOfStates;
	string reachable, symbols;
	vector<State> states;
	vector<vector<State>> sets;
	vector<State> nonFinal;
	vector<State> final;
public:
	DFA() {
		noOfSymbols = noOfStates = 0;
		reachable = "";
	}

	void setSymbols(int n) {
		noOfSymbols = n;
	}

	string split(string s)
	{
		string word;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] != ',' && s[i]!=' ')
				word += s[i];
		}
		return word;
	}

	string _split(string s) {
		string word;
		bool flag = 0;
		for (int i = 0; i < s.length(); i++) {
			if (!flag && s[i] == ',')
				flag = 1;
			if (s[i] != ' ' && s[i] != ',' && flag)
				word += s[i];
		}
		return word;
	}

	void writeFile() {
		ofstream file("output.txt");
		string sym = "States/Input Symbols, ";
		for (int i = 0; i < symbols.length(); i++) {
			sym += symbols[i];
			if (i != symbols.length() - 1)
				sym += ", ";
		}
		file << sym << endl;
		sym = "";
		for (State s : states) {
			if (s.isinitial())
				sym += "i";
			if (s.isfinal())
				sym += "*";
			sym += s.getname();
			vector<string> points = s.getpoints();
			for (auto it = points.begin(); it != points.end(); it++) {
				sym += ", ";
				sym += *it;
			}
			file << sym << endl;
			sym = "";
		}
	}

	void removeDuplicates() {
		set<State> set(states.begin(), states.end());
		states.assign(set.begin(), set.end());
	}

	void updateStates(vector<vector<State>> current) {
		for (int s = 0; s < states.size();s++) {
			for (vector<State> set : current) {
				string nName = "";
				if (set.size() > 1) {
					for (int i = 0; i < set.size(); i++) {
						nName += set[i].getname();
					}
					if (strstr(nName.c_str(), states[s].getname().c_str())) {
						states[s].setname(nName);
					}
					vector<string> pointer = states[s].getpoints();
					for (auto it = pointer.begin(); it < pointer.end();++it) {
						string pName = *it;
						if (strstr(nName.c_str(), pName.c_str())) {
							states[s].updatepoints(nName,pName);
						}
					}
				}
			}
		}
	}

	bool found(State s1, vector<vector<State>> current) {
		for (vector<State> set : current) {
			for (State s : set) {
				if (s.getname() == s1.getname())
					return true;
			}
		}
		return false;
	}

	bool checkPoints(State s1, State s2, vector<vector<State>> prev) {
		vector<string> point1 = s1.getpoints();
		vector<string> point2 = s2.getpoints();
		int found = 0;
		for (auto it = point1.begin(), it2 = point2.begin(); it != point1.end(); ++it, ++it2) {
			int flag = 0;
			for (vector<State>set : prev) {
				for (State s : set) {
					if (flag)
						break;
					if (s.getname() == *it) {
						found++;
						for (State ss: set) {
							if (ss.getname() == *it2) {
								found++;
								flag = 1;
								break;
							}
						}
					}

				}
			}
		}
		return found==noOfSymbols*2;
	}

	void partition() {
		for (State s : states) {
			if (s.isfinal())
				final.push_back(s);
			else
				nonFinal.push_back(s);
		}
		sets.push_back(nonFinal);
		sets.push_back(final);
		vector<vector<State>> prev;
		vector<vector<State>> current;
		current = sets;
		cout << "Partitions:\n";
		while (prev.size()!=current.size()) {
			prev.clear();
			prev = current;
			current.clear();
			for (vector<State> set : prev) {
				if (set.size() > 1) {
					for (State s1 : set) {
						vector<State> temp;
						if (!found(s1, current)) {
							temp.push_back(s1);
							for (State s2 : set) {
								if ((s2.getname() != s1.getname()) && (s1.isfinal() == s2.isfinal())
									&& (!found(s2, current)) && (checkPoints(s1, s2, prev))) {
									temp.push_back(s2);
								}
							}
							current.push_back(temp);
						}
					}
				}
				else
					current.push_back(set);
			}
			cout << "{";
			for (int i = 0; i < prev.size();i++) {
				vector<State> vec = prev[i];
				cout << "{";
				for (int j = 0; j < vec.size();j++) {
					cout << vec[j].getname();
					if (j + 1 != vec.size()) {
						cout << ",";
					}
				}
				cout << "}";
			}
			cout << "}" << endl;
		}
		updateStates(current);
	}

	void removeUnreachable() {
		for (State s : states) {
			bool flag = 0;
			string st;
			for (int i = 0; i < reachable.length(); i++) {
				st.clear();
				st.push_back(reachable[i]);
				if (s.getname() == st) {
					flag = 1;
					break;
				}
			}
			if (!flag && !s.isinitial()) {
				vector <State>::iterator it;
				for (it = states.begin(); it != states.end(); ++it) {
					if (it->getname() == s.getname()) {
						it = states.erase(it);
						--it;
					}
				}
			}
		}
	}

	void readFile(string fname) {
		ifstream fp(fname, ios::in);
		if (fp) {
			int loc = 0;
			string symbols;
			getline(fp, symbols);
			symbols = _split(symbols);
			this->symbols = symbols;
			noOfSymbols = symbols.length();
			while (fp.good()) {
				vector<string> stateMap;
				noOfStates++;
				bool final = 0, initial = 0;
				string row, name;
				getline(fp, row);
				row = split(row);
				if (row.length() == noOfSymbols + 2) {
					if (row[0] == '*')
						final = 1;
					else
						initial = 1;
					name = row[1];
					for (int i = 2, j = 0; i < row.length(); i++, j++) {
						string toPoint(1, row[i]);
						stateMap.push_back(toPoint);
						reachable += toPoint;
					}
				}
				else if (row.length() > noOfSymbols + 2) {
					final = initial = 1;
					name = row[2];
					for (int i = 3, j = 0; i < row.length(); i++, j++) {
						string toPoint(1, row[i]);
						stateMap.push_back(toPoint);
						reachable += toPoint;
					}
				}
				else {
					name = row[0];
					for (int i = 1, j = 0; i < row.length(); i++, j++) {
						string toPoint(1, row[i]);
						stateMap.push_back(toPoint);
						reachable += toPoint;
					}
				}
				State obj(name,final,initial,stateMap);
				states.push_back(obj);
			}
		}
	}

	void minimizeDFA() {
		readFile("input.txt");
		removeUnreachable();
		partition();
		removeDuplicates();
		writeFile();
		cout << "\nOutput printed in output.txt\n";
	}
};