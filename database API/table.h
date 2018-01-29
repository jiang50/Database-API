#include "record.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
using namespace std;

class attribute{
	public:
	attribute(string name,string type):n(name),t(type){}
	string get_name(){return n;}
	string get_type(){return t;}
	private:
	string n;
	string t;
};


class Table {
	private:
	
	
	string key;
	public:
	Table(){}
	Table(vector<attribute> a); 	
	vector<vector<string>> t;//data table
	unordered_map<string,string> attr;//hash table of attributes, key is name value is type
	void AddAttribute(attribute a);//: initial entries with NULL
	void DeleteAttribute(string a);
	void insert_record(Record r);

	vector<string> get_attr();

	int get_size(); //: return the number of records
	int get_index(string s);
	vector<Record> iterate();
	void specifyKey(string attr);
	string get_key(){return key;}
	Table cross_join(Table a, Table b);
	Table natural_join(Table a, Table b);
	int count(string attr);
	string Min(string a);
	string Max(string a);
	void print();
};