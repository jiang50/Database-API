#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Record {
	public:
	vector<string> entries;

	Record(vector<string> record){
        entries = record;
    } // Record(List<string>)
	Record(int n){
        for(int i = 0; i < n; ++i)
            entries.push_back("");
    }
	
	int size(){
        return entries.size();
    }
	
	string get(int i){
        if(i >= entries.size())
            return "out of boundary";
        return entries[i];
    }
	
	void set(int i, string val){
        if(i >= entries.size()){
            cout<< "out of boundary";
            return;
        }
        entries[i] = val;
    }
};

