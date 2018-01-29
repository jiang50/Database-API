#include "table.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <algorithm>
Table::Table(vector<attribute> a){
	vector<string> att;
	for(int i=0;i<a.size();i++){
		if(attr.count(a[i].get_name())){
			cout<<"duplicate attributes!"<<endl;
			break;
		}
		else {
			attr[a[i].get_name()]=a[i].get_type();
			att.push_back(a[i].get_name());
		}
	}
	t.push_back(att);
//	print();
}

void Table::AddAttribute(attribute a){
	if(attr.count(a.get_name())){
		cout<<"Already exist attribute "<<a.get_name()<<".\n";
		return;
	}
	else{
		attr[a.get_name()]=a.get_type();
		if(t.size()==0)t.push_back({a.get_name()});
		else t[0].push_back(a.get_name());
		for(int i=1;i<t.size();i++){
			t[i].push_back("NULL");
		}
	}
//	print();
}

void Table::DeleteAttribute(string a){

	int index=get_index(a);
/*	for(int i=0;i<t[0].size();i++){
		if(t[0][i]==a){
			index=i;
			break;
		}
	}*/
	if(index==-1){
		cout<<"There is no attribute "<<a<<" in the table.\n";
		return;
	}
	attr.erase(a);
	for(int j=0;j<t.size();j++){
		t[j].erase(t[j].begin()+index);
	}
//	print();
}

int Table::get_index(string s){
	
	for(int i=0;i<t[0].size();i++){
		if(t[0][i]==s)return i;
	}
	return -1;
}

void Table::insert_record(Record r){
	vector<string> s=r.entries;
	if(s.size()!=t[0].size()){
		cout<<"Cannot insert this record because the number of attributes is different."<<endl;
		return;
	}
	if(key.size()>0){
		
		int index=get_index(key);
/*		for(int i=0;i<t[0].size();i++){
			if(t[0][i]==key){
				index=i;
				break;
			}
		}*/

		for(int i=0;i<t.size();i++){
			if(t[i][index]==s[index]){
				cout<<"Cannot insert this record because of duplicate key!"<<endl;
				return;
			}
			
		}
	}
	
	for(int i=0;i<s.size();i++){
		if(attr[t[0][i]]=="int"){
			stringstream sstr(s[i]);
			int a;
			sstr>>a;
			if(sstr.fail()){
				cout<<"The data type of new record doesn't match"<<endl;
				return;
			}
		}
		else if(attr[t[0][i]]=="double"){
			stringstream sstr(s[i]);
			double a;
			sstr>>a;
			if(sstr.fail()){
				cout<<"The data type of new record doesn't match"<<endl;
				return;
			}
		}
	}
	t.push_back(s);
//	print();
}

vector<string> Table::get_attr(){
	return t[0];
}

int Table::get_size(){
	return t.size()-1;
}

vector<Record> Table::iterate(){
	vector<Record> res;
	for(int i=1;i<t.size();i++){
		Record r(t[i]);
		res.push_back(r);
	}
	return res;
}

void Table::specifyKey(string attr){
	int index=get_index(attr);
/*	for(int i=0;i<t[0].size();i++){
		if(t[0][i]==attr){
			index=i;
			break;
		}
	}*/
	if(index==-1){
		cout<<"There is no attribute "<<attr<<" in the table.\n";
		return;
	}
	unordered_set<string> s;
	for(int i=0;i<t.size();i++){
		if(s.count(t[i][index])){
			cout<<attr<<" cannot be key!"<<endl;
			return;
		}
		else s.insert(t[i][index]);
	}
	key=attr;
}



void Table::print(){
	for(int i=0;i<t.size();i++){
		for(int j=0;j<t[0].size();j++){
			cout<<t[i][j]<<'\t';
		}
		cout<<endl;
	}
	cout<<endl;
}

Table Table::cross_join(Table a, Table b){
	if(b.t.size()==0)return a;
	if(a.t.size()==0)return b;
	Table result;
	vector<vector<string>> re;
	vector<string> att=a.t[0];
	unordered_map<string,string> m=a.attr;
	for(int i=0;i<b.t[0].size();i++){
		if(m.count(b.t[0][i])){
			att.push_back(b.t[0][i]+"2");
			m[b.t[0][i]+"2"]=b.attr[b.t[0][i]];
		}
		else{
			att.push_back(b.t[0][i]);
			m[b.t[0][i]]=b.attr[b.t[0][i]];
			
		}
	}
	re.push_back(att);
	
	for(int i=1;i<a.t.size();i++){
		
		for(int j=1;j<b.t.size();j++){
			vector<string> newv=a.t[i];
			newv.insert(newv.end(),b.t[j].begin(),b.t[j].end());	
			re.push_back(newv);			
		}
		
	}
	result.t=re;
	result.attr=m;
//	result.print();
	return result;
}

Table Table::natural_join(Table a, Table b){
	if(b.get_key().size()==0){
		perror("Cannot join!");
		return a;
	}
	if(!a.attr.count(b.get_key())){
		perror("Cannot join!");
		return a;
	}
	int index_b=b.get_index(b.get_key());
	int index_a=a.get_index(b.get_key());
	vector<int>id_col;
	for(int i=0;i<b.t[0].size();i++){
		if(!a.attr.count(b.t[0][i])){
			a.t[0].push_back(b.t[0][i]);
			id_col.push_back(i);
		}
	}
	for(int i=1;i<a.t.size();i++){
		string k=a.t[i][index_a];
//		cout<<k<<endl<<endl;
		bool find=true;
		for(int j=1;j<b.t.size();j++){
//			cout<<b.t[j][index_b]<<endl;
			if(b.t[j][index_b]==k){
				find=false;
				for(int k:id_col){
					a.t[i].push_back(b.t[j][k]);
				}
				break;
			}
		}
		if(find){
			for(int k :id_col)a.t[i].push_back("NULL");
		}
	}
	return a;
}

int Table::count(string attr){
	int index=get_index(attr);
	int count=0;
	if(index==-1){
		perror("Attribute doesn't exist!");
		return 0;
	}
	for(int i=1;i<t.size();i++){
		if(t[i][index]!="NULL")count++;
		
	}
	return count;
}

string Table::Min(string a){
	int index=get_index(a);
	
	if(attr[a]=="int"){
		vector<int> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				stringstream sstr(s);
				int a;
				sstr>>a;
				v.push_back(a);
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return to_string(v[0]);
		}
		
	}
	
	else if(attr[a]=="double"){
		vector<double> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				stringstream sstr(s);
				double a;
				sstr>>a;
				v.push_back(a);
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return to_string(v[0]);
		}
		
	}
	
	else{
		vector<string> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				v.push_back(s);
				
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return v[0];
		}
		
	}
}



string Table::Max(string a){
	int index=get_index(a);
	
	if(attr[a]=="int"){
		vector<int> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				stringstream sstr(s);
				int a;
				sstr>>a;
				v.push_back(a);
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return to_string(v[v.size()-1]);
		}
		
	}
	
	else if(attr[a]=="double"){
		vector<double> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				stringstream sstr(s);
				double a;
				sstr>>a;
				v.push_back(a);
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return to_string(v[v.size()-1]);
		}
		
	}
	
	else{
		vector<string> v;
		for(int i=1;i<t.size();i++){
			string s=t[i][index];
			if(s!="NULL"){
				v.push_back(s);
				
			}
			
		}
		if(v.size()==0){
			cout<<"Doesn't exist Min"<<endl;
			return "";
		}
		else{
			sort(v.begin(),v.end());
			return v[v.size()-1];
		}
		
	}
}






















