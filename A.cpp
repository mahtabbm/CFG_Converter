#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum linear_type
{
	Right, Left
};

class LinearGrammar{

	linear_type type;
	int n_rules;
	string variables;
	string start;
	string terminals;

	void filling_terminals_variables();
	void determining_start();
	vector< pair<string, string> > converting_left_to_right();
	vector< pair<string, string> > converting_right_to_left();
	void which_type();

public:

	vector< pair<string, string> > rules;

	LinearGrammar(){}

	LinearGrammar(vector< pair<string, string> > rules):rules(rules){
		n_rules = rules.size();
		filling_terminals_variables();
		which_type();
		determining_start();
	}

	LinearGrammar converting();

	int getN(){
		return n_rules;
	}

	friend ostream& operator << (ostream&, const LinearGrammar&);

};

ostream& operator << (ostream& os, const LinearGrammar& lg){
	for (int i = 0; i < lg.n_rules; ++i)
	{
		os << lg.rules[i].first << " -> " << lg.rules[i].second << endl;
	}
	return os;
}

void LinearGrammar::which_type(){
	for (int i = 0; i < n_rules; ++i)
	{
		int len = rules[i].second.length();
		if(variables.find(rules[i].second[0]) != string::npos){
			type = Left;
			return;
		}
		else if(variables.find(rules[i].second[len-1]) != string::npos){
			type = Right;
			return;
		}
	}
}

void LinearGrammar::determining_start(){

	int n = variables.length();

	for (int i = 0; i < n; ++i)
	{
		int n_occured = 0;
		for (int j = 0; j < n_rules; ++j)
			if(rules[j].second.find(variables[i]) != string::npos)
				n_occured++;
		if( n_occured == 0 ){
			start = variables[i];
			return;
		}
	}
	start = "$";
	pair<string, string> p; p.first = start; p.second = variables[0];
	rules.push_back(p);
	n_rules++;
}

void LinearGrammar::filling_terminals_variables(){
	for (int i = 0; i < n_rules; ++i)
	{
		if(terminals.find(rules[i].first[0]) == string::npos){
			if( ('a' <= rules[i].first[0] && rules[i].first[0] <= 'z') 
				 || ('0' <= rules[i].second[j] && rules[i].second[j] <= '9')){
				terminals += rules[i].first[0];
			}
			else if('A' <= rules[i].first[0] && rules[i].first[0] <= 'Z'){
				variables += rules[i].first[0];
			}
		}
		int len = rules[i].second.length();
		for (int j = 0; j < len; ++j)
		{
			if(terminals.find(rules[i].second[j]) == string::npos){
				if( ('a' <= rules[i].second[j] && rules[i].second[j] <= 'z')
					|| ('0' <= rules[i].second[j] && rules[i].second[j] <= '9') ){
					terminals += rules[i].second[j];
				}
				else if('A' <= rules[i].second[j] && rules[i].second[j] <= 'Z'){
					variables += rules[i].second[j];
				}
			}
		}
	}
}

vector< pair<string, string> > LinearGrammar::converting_right_to_left(){
	vector< pair<string, string> > new_rules;
	for (int i = 0; i < n_rules; ++i)
	{
		pair<string, string> p;
		if(variables.find(rules[i].first) != string::npos){
			int len = rules[i].second.length();
			if(rules[i].first == start){
				if( terminals.find(rules[i].second[len-1]) != string::npos){
					new_rules.push_back(rules[i]);
				}
				else{
					p.first = rules[i].second[len-1];
					p.second = rules[i].second.substr(0, len-1);
					new_rules.push_back(p);
				}
			}
			else{
				if(variables.find(rules[i].second[len-1]) != string::npos){
					p.first = rules[i].second[len-1];
					p.second = rules[i].first + rules[i].second.substr(0, len-1);
					new_rules.push_back(p);
				}
				else{
					p.first = start;
					p.second = rules[i].first + rules[i].second;
					new_rules.push_back(p);
				}
			}
		}
	}
	return new_rules;
}

vector<pair<string, string> > LinearGrammar::converting_left_to_right(){
	vector< pair<string, string> > new_rules;
	for (int i = 0; i < n_rules; ++i)
	{
		pair<string, string> p;
		if(rules[i].first == start){
			if(terminals.find(rules[i].second[0]) != string::npos){
				new_rules.push_back(rules[i]);
			}
			else if(variables.find(rules[i].second[0]) != string::npos){
				if(rules[i].second.length() == 1){
					p.first = rules[i].second;
					p.second = "";
					new_rules.push_back(p);
				}
				else{
					p.first = rules[i].second[0];
					p.second = rules[i].second.substr(1);
					new_rules.push_back(p);
				}
			}
		}
		if(rules[i].first != start){
			if(variables.find(rules[i].first) != string::npos){
				if(terminals.find(rules[i].second[0]) != string::npos){
					p.first = start;
					p.second = rules[i].second + rules[i].first;
					new_rules.push_back(p);
				}
				else if(variables.find(rules[i].second[0]) != string::npos){
					p.first = rules[i].second[0];
					p.second = rules[i].second.substr(1) + rules[i].first;
					new_rules.push_back(p);
				}
			}
		}
	}
	return new_rules;
}

LinearGrammar LinearGrammar:: converting(){
	vector<pair<string, string> > converted_vec;
	switch(type){
	case 0:
		converted_vec = converting_right_to_left();
	break;
	case 1:
		converted_vec = converting_left_to_right();
	break;
	}
	LinearGrammar converted(converted_vec);
	return converted;
}

int main(){
	std::vector< pair<string, string> > v;
	pair<string, string> p;
	int n; cin >> n;
	while(n--){
		cin >> p.first;
		string spam; cin >> spam;
		cin >> p.second;
		if(p.second == "-1") {
			p.second = "";
			v.push_back(p);
			break;
		}
		v.push_back(p);
	}
	LinearGrammar con(v);
	con = con.converting();
	cout << con;
	return 0;
}