#include "table.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>      // std::istringstream
#include <cmath>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <map>


using namespace std;
class Database{
private:
	unordered_map <string, Table> temp_tablelist;
	
	bool table_exist(string table)
	{
		for(auto iter = tablelist.begin(); iter != tablelist.end(); ++iter){
			if(iter->first == table)
				return true;
		}
		return false;
	}
	
	bool temp_table_exist(string table)
	{
		for(auto iter = temp_tablelist.begin(); iter != temp_tablelist.end(); ++iter){
			if(iter->first == table)
				return true;
		}
		return false;
	}
	
	//extract the contend in parenthese
	string extract_content_paren(string s)
	{
		int count = 0;
		bool is_first = true;
		int begin = 0;
		int end = 0;
		for(int i = 0; i < s.length(); i++){
			if(s[i] == '('){
				++count;
				if(is_first){
					is_first = false;
					begin = i;
				}
			}
			else if (s[i] == ')'){
				--count;
				if(count ==0)
					end = i;
			}
		}
		if(count != 0){
			//cout << "imbalance parenthesis" << endl;
			return "";
		}
		//cout<<"begin: " << begin <<"end: " << end << endl;
		return s.substr(begin, end-begin);
	}

public:
	unordered_map <string, Table> tablelist;

	Database(){};

	void add(Table a, string name){
		tablelist.insert(pair<string,Table>(name,a));
	}

	void drop(string name){
		tablelist.erase(name);
	}

	vector<Table> get_alltable(){
		vector<Table> t1;
		for(auto it=tablelist.begin();it!=tablelist.end();++it){
			t1.push_back(it->second);
		}
		return t1;
	}

	vector<string> get_alltableName(){
		vector<string> t2;
		for(auto it=tablelist.begin();it!=tablelist.end();++it){
			t2.push_back(it->first);
            //cout<< " name: " << it->first << endl;
		}
		return t2;
	}

	Table query_process(vector<string> att, string table, string where)
	{
		//check if table exist
		if(!table_exist(table)){
			throw "table does not exist";
		}
        //check if attribute exist
        for(int i = 0; i < att.size(); ++i){
            if(!is_attribute(table, att[i])){
                throw "attribute does not exist";
            }
        }
        Table t;
//		//process parenthesis
//		string con = extract_content_paren(where);
//		//cout << con << endl;
		
		vector<string> c;
		istringstream iss(where);
		for(string s; iss >> s;)
			c.push_back(s);
		
		//split the conditions based on AND/OR/NOT
		vector<string> conditions;
		string s;
		for(int i = 0; i < c.size(); ++i){
			if(c[i] == "AND" || c[i] == "OR" || c[i] == "NOT"){
				conditions.push_back(s);
				conditions.push_back(c[i]);
				s = "";
			}
			else{
				s = s + " "+ c[i];
			}
		}
		conditions.push_back(s); //add the last condition
		
		 for(int i = 0; i < conditions.size(); ++i){
			 cout << conditions[i] << endl;
		 }
		
		vector<string> res;
		for(int i = 0; i < conditions.size(); ++i){
            if (conditions[i] == " " || conditions[i] == "")
                continue;
			if(!(conditions[i] == "AND" || conditions[i] == "OR" || conditions[i] == "NOT")){
				//if(!is_table(condition[i])
				vector<string> test = infix_to_postfix(conditions[i]);
                cout << "test: ";
                for(int i = 0; i < test.size(); ++i)
                    cout << test[i] << " ";
                cout << endl;
//                for(int i = 0; i < res.size(); ++i)
//                    cout << res[i] << " ";
				res.push_back(eval_postfix(table, test));
			}
            else{
                res.push_back(conditions[i]);
            }
		}
		
//		cout << "test: ";
//		for(int i = 0; i < test.size(); ++i)
//			cout << test[i] << " ";
//		cout << endl;
//		
//		eval_postfix(table, test);
//        for(int i = 0; i < res.size(); ++i)
//            cout << res[i] << " ";
//        cout << endl;
        
        vector<string> res1 = infix_to_postfix_logic(res);
//        for(int i = 0; i < res1.size(); ++i)
//            cout << res1[i] << " ";
//        cout << endl;
        
        string result_index = eval_postfix_logic(table, res1);

//        cout << result_index << endl;
        
        //build table
        
        for(int i = 0; i < att.size(); i++){
            attribute a(att[i], tablelist[table].attr[att[i]]);
            t.AddAttribute(a);
        }
        
        vector<int> index;
        istringstream i(result_index);
        for(int in; i >> in;)
            index.push_back(in);
        
        for(int i = 0; i < index.size(); ++i){
            vector<string> record;
            for(int j = 0; j < att.size(); j++){
                int att_index = tablelist[table].get_index(att[j]);
                record.push_back(tablelist[table].t[index[i]][att_index]);
            }
            t.insert_record(record);
        }
        
        t.print();
        return t;
	}
	
	//process the condition in the parenthese, return a table name
//	string query_parenthese_eval(string table, string where)
//	{
//		return "";
//	}
	
	vector<string> infix_to_postfix(string i)
	{
		vector<string> in;
		istringstream iss(i);
		for(string s; iss >> s;)
			in.push_back(s);
		
		vector<string> postfix;
		stack<string> stack;
		//string previous;
		int openParens = 0;
		
		for(int i = 0; i < in.size(); ++i){
			// if(is_number(in[i]) || in[i].at(0) == "\""){ //number or string
				// postfix.push_back(in[i]);
			// }
			if(is_operator(in[i])){
				if(stack.empty()){
					stack.push(in[i]);
				}
				else{
					 string op = stack.top();
					 while(!stack.empty() && precedence(op) > precedence(in[i])){
						op = stack.top();
						stack.pop();
						postfix.push_back(op);
					 }
					stack.push(in[i]);
				}
			}
            else if(in[i] == "("){
                openParens++;
                stack.push(in[i]);
            }
            else if(in[i] == ")"){
                openParens--;
                while(!stack.empty() && stack.top() !="("){
                    postfix.push_back(stack.top());
                    stack.pop();
                }
            }
			else{  //attribute
				postfix.push_back(in[i]);
			}
		}
		while (!stack.empty()) {
			string op = stack.top();
			stack.pop();
			postfix.push_back(op);
		}
		
		// for(int i = 0; i < in.size(); ++i){
			// cout << in[i] << endl;
		// }
//		for(int i = 0; i < postfix.size(); ++i){
//			cout << postfix[i] << endl;
//		}
//		cout << "postfix size: " << postfix.size() << endl;
		return postfix;
	}
    
    vector<string> infix_to_postfix_logic(vector<string> c)
    {
        vector<string> res;
        stack<string> stack;
        for(int i = 0; i < c.size(); ++i){
            if(is_logic(c[i])){
                if(stack.empty()){
                    stack.push(c[i]);
                }
                else{
                    string op = stack.top();

                    while((!stack.empty()) && logic_precedence(op) > logic_precedence(c[i])){
                        op = stack.top();
                        stack.pop();
                        res.push_back(op);
                    }
                    stack.push(c[i]);
                }
            }
            else{
                res.push_back(c[i]);
            }
        }
        while(!stack.empty()){
            string op = stack.top();
            stack.pop();
            res.push_back(op);
        }
        return res;
    }
	
	string eval_postfix(string table, vector<string>& cond_postfix)
	{
        string logic_o;
        logic_o = cond_postfix[cond_postfix.size() - 1];
        
        if(logic_o != "=" && logic_o != "<>" && logic_o != ">" && logic_o != "<" && logic_o != ">=" && logic_o != "<=")
            throw "an expression must end with logical operation";
        
		stack<string> stack;
		
		for(int i = 0; i < cond_postfix.size(); ++i){
			if(is_operator(cond_postfix[i])){
				if(stack.empty()){
                    throw "error in eval_postfix 1";
				}
				string a = stack.top();
				stack.pop();
				
				if(stack.empty()){
					throw "error in eval_postfix 2";
				}
				string b = stack.top();
				stack.pop();
				
				string res = b + a + cond_postfix[i];
				//cout << "eval_postfix: " << res << endl;
				
				//call function to evaluate res, and then return the name and push to the stack
				res = eval_expression(table, b, a, cond_postfix[i]);
                stack.push(res);
			}
			else{
				stack.push(cond_postfix[i]);
			}
			//parse res to a function that build the table based on the condition
		}
        if(stack.empty()){
            throw "something wrong in eval_postfix";
        }
        return stack.top();
	}
    
    string eval_postfix_logic(string table, vector<string> logic_postfix)
    {
        stack<string> stack;
        
//        for(int i = 0; i < logic_postfix.size(); ++i)
//            cout << logic_postfix[i] << endl;
        
        for(int i = 0; i < logic_postfix.size(); ++i){
            if(is_logic(logic_postfix[i])){
                string res;
                
                if(logic_postfix[i] == "NOT"){
                    if(stack.empty()){
                        throw "error in eval_postfix_logic1";
                    }
                    //cout << "come here" << endl;
                    string a = stack.top();
                    stack.pop();
                    
                    res = eval_expression_logic(table, a, " ", logic_postfix[i]);
                }
                else{
                    if(stack.empty()){
                        throw "error in eval_postfix_logic2";
                    }
                    string a = stack.top();
                    stack.pop();

                    if(stack.empty()){
                        throw "error in eval_postfix_logic3";
                    }
                    string b = stack.top();
                    stack.pop();

                    res = eval_expression_logic(table, b, a, logic_postfix[i]);
                }
                stack.push(res);
            }
            else{
                stack.push(logic_postfix[i]);
            }
        }
        if(stack.empty()){
            throw "something wrong in eval_postfix";
        }
        return stack.top();
    }
	
	string eval_expression(string table, string a, string b, string op)
	{
		string res;
		
		if(op == "*" || op == "/" || op == "+" || op == "-"){
			cout << "arithmetic eval" << endl;
			res = eval_arithmetic(table, a, b, op);
		}
		else if(op == "=" || op == "<>" || op == ">" || op == "<" || op == ">=" || op == "<="){
			cout << "logical eval" << endl;
            res = eval_logical(table, a, b, op);
		}
		else
			throw "unknown operator";
		return res;
	}
	
    string eval_expression_logic(string table, string a, string b, string op)
    {
        if(op == "NOT"){
            string res;
            
            map<string, int> map;
            
            vector<string> v;
            istringstream iss(a);
            for(string s; iss >> s;)
                v.push_back(s);
            
            for(int i = 1; i < tablelist[table].t.size(); ++i){
                map[to_string(i)] = 1;
            }
            
            for(int i = 0; i < v.size(); ++i)
                map[v[i]] = 0;
            
            for(auto it = map.begin(); it != map.end(); ++it){
                if(it->second > 0){
                    res = res + " " + it->first;
                }
            }
            return res;
        }
        
        map<string, int> map;
		istringstream iss(a);
		for(string s; iss >> s;)
			map[s] = 1;
        
        string res;
        
        if(op == "AND"){
            istringstream is(b);
            for(string s; is >> s;){
                if(map.find(s) != map.end()){
                    map[s] = map[s] + 1;
                }
            }
            for(auto it = map.begin(); it != map.end(); ++it){
                if(it->second > 1){
                    res = res + " " + it->first;
                }
            }
        }
        else if(op == "OR"){
            istringstream is(b);
            for(string s; is >> s;){
                map[s] = 1;
            }
            for(auto it = map.begin(); it != map.end(); ++it){
                res = res + " " + it->first;
            }
        }
        return res;
    }
    
	string eval_arithmetic(string table, string a, string b, string op)
	{
		if(is_number(a)){
			//cout << "a is number" << endl;
			if(is_number(b)){
				//cout << "b is a number" << endl;
				
				double a1 = atof(a.c_str());
				double b1 = atof(b.c_str());
				double res;
				if(op == "+"){
					res = a1 + b1;
				}
				else if(op == "-"){
					res = a1 - b1;
				}
				else if(op == "*"){
					res = a1 * b1;
				}
				else{
					res = a1 / b1;
				}
				//cout << res << endl;
				return to_string(res);
			}
			
			else if(is_attribute(table, b)){
				//cout << "b is attribute" << endl;
				
				//check if the attribute type is correct
				if(tablelist[table].attr[b] != "int" && tablelist[table].attr[b] != "double"){
					throw "the type of attribute is incorrect";
				}
				
				int index = tablelist[table].get_index(b);
				string res = tablelist[table].t[0][index];
				for(int i = 1; i < tablelist[table].t.size(); ++i){
					////cout << tablelist[table].t[i][index] << endl;
					
                    string result;
                    double a1 = atof(a.c_str());
					double b1 = atof(tablelist[table].t[i][index].c_str());
					if(op == "+"){
						result = to_string(a1 + b1);
					}
					else if(op == "-"){
						result = to_string(a1 - b1);
					}
					else if(op == "*"){
						result = to_string(a1 * b1);
					}
					else{
						result = to_string(a1 / b1);
					}
					
					res = res + " " + result;
				}
				return res;
			}
			
			else{
				//cout << "b cannot be identified" << endl;
				return "";
			}
		}
		
		else if (a.at(0) == '\"' && a.at(a.length()-1) == '\"'){
			//cout << "a is string" << endl;
			//cout << "it is meaningless to do arithmetic to string" << endl;
			return "";
		}
		
		else if (is_attribute(table, a)){
			//cout << "a is attribute" << endl;
			if(is_number(b)){
				//cout << "b is a number" << endl;
				
				//check if the attribute type is correct
				if(tablelist[table].attr[a] != "int" && tablelist[table].attr[a] != "double"){
					throw "the type of attribute is incorrect";
				}
				
				int index = tablelist[table].get_index(a);
				string res = tablelist[table].t[0][index];
				for(int i = 1; i < tablelist[table].t.size(); ++i){
					////cout << tablelist[table].t[i][index] << endl;
                    string result;
					double a1 = atof(tablelist[table].t[i][index].c_str());
                    double b1 = atof(b.c_str());
					if(op == "+"){
						result = to_string(a1 + b1);
					}
					else if(op == "-"){
						result = to_string(a1 - b1);
					}
					else if(op == "*"){
						result = to_string(a1 * b1);
					}
					else{
						result = to_string(a1 / b1);
					}
					
					res = res + " " + result;
				}
				return res;
			}
			else if(is_attribute(table, b)){
				//cout << "b is a attribute" << endl;
				//cout << "it is meaningless to do arithmetic to attribute and attribute" << endl;
				return "";
			}
			else{
				//cout << "cannot identify b";
			}
		}
		
		else{
			throw "cannot identify A";
        }
		return "";
	}
	
	string eval_logical(string table, string a, string b, string op)
	{
        cout << "a: " << a << " b: " << b << endl;
		if(is_attribute(table, a)){
			cout << " a is an attribute" << endl;
			if(is_number(b)){
				cout << "b is a number" << endl;
				int index = tablelist[table].get_index(a);
				//check if the attribute is a number
				if(tablelist[table].attr[a] != "int" && tablelist[table].attr[a] != "double"){
					throw "the type of attribute is incorrect";
				}
				string indexes;
				for(int i = 1; i < tablelist[table].t.size(); ++i){
                    double a1 = atoi(tablelist[table].t[i][index].c_str());
                    double b1 = atoi(b.c_str());
                    
					if(op == "="){
						if(a1 == b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<>"){
						if(a1 != b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == ">"){
						if(a1 > b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<"){
						if(a1 < b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == ">="){
						if(a1 >= b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<="){
						if(a1 <= b1)
							indexes = indexes + " " + to_string(i);
					}
				}
				return indexes;
			}
			else if(is_attribute(table,b)){
				cout << "b is an attribute" << endl;
				//check if these two attributes has the same type and if they can compare
				if(tablelist[table].attr[b] != tablelist[table].attr[a]){
					throw "the type of attributes are not the same";
				}
				if(tablelist[table].attr[a] == "int" || tablelist[table].attr[a] == "double"){
					string indexes;
					int index_a = tablelist[table].get_index(a);
					int index_b = tablelist[table].get_index(b);
                    
					for(int i = 0; i < tablelist[table].t.size(); ++i){
                        double a1 = atoi(tablelist[table].t[i][index_a].c_str());
                        double b1 = atoi(tablelist[table].t[i][index_b].c_str());
						if(op == "="){
							if(a1 == b1)
								indexes = indexes + " " + to_string(i);
						}
						else if(op == "<>"){
							if(a1 != b1)
								indexes = indexes + " " + to_string(i);
						}
						else if(op == "<"){
							if(a1 < b1)
								indexes = indexes + " " + to_string(i);
						}
						else if(op == ">"){
							if(a1 > b1)
								indexes = indexes + " " + to_string(i);
						}
						else if(op == "<="){
							if(a1 <= b1)
								indexes = indexes + " " + to_string(i);
						}
						else if(op == ">="){
							if(a1 >= b1)
								indexes = indexes + " " + to_string(i);
						}
					}
					return indexes;
				}
				//string type
				else{
					cout << "b is a string" << endl;
					string indexes;
					int index_a = tablelist[table].get_index(a);
					int index_b = tablelist[table].get_index(b);
					for(int i = 0; i < tablelist[table].t.size(); ++i){
						if(op == "="){
							if(tablelist[table].t[i][index_a] == tablelist[table].t[i][index_b])
								indexes = indexes + " " + to_string(i);
						}
						else if(op == "<>"){
							if(tablelist[table].t[i][index_a] != tablelist[table].t[i][index_b])
								indexes = indexes + " " + to_string(i);
						}
						else{
							//cout << "string attribute cannot do operations other than equality";
							break;
						}
					}
					return indexes;
				}
			}
			else{
				//maybe it is a string with the results
				istringstream iss(b);
				vector<string> r;
				for(string s; iss>>s;)
					r.push_back(s);
				
				if(r.size() > 0 && is_attribute(table, r[0])){
                    cout << "b is the data from attribute after arithmetic operation" << endl;
					//check if these two attributes has the same type and if they can compare
					if(tablelist[table].attr[a] != tablelist[table].attr[r[0]]){
						throw "the type of attributes are not the same";
					}
                    if(tablelist[table].t.size() != r.size()){
                        throw "the size of these two attribute are not the same";
                    }
					
                    int index = tablelist[table].get_index(a);
					//if these two attributes are number
					if(tablelist[table].attr[a] == "int" || tablelist[table].attr[a] == "double"){
						string indexes;
						for(int i = 1; i < r.size(); ++i){
                            double a1 = atoi(a.c_str());
                            double b1 = atoi(tablelist[table].t[i][index].c_str());
							if(op == "="){
								if(a1 == b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<>"){
								if(a1 != b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == ">"){
								if(a1 > b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<"){
								if(a1 < b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == ">="){
								if(a1 >= b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<="){
								if(a1 <= b1)
									indexes = indexes + " " + to_string(i);
							}
						}
                        return indexes;
					}
					
					//if these two attribute are strings
					else{
						string indexes;
						for(int i = 1; i < r.size(); ++i){
							if(op == "="){
								if(tablelist[table].t[i][index] == r[i])
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<>"){
								if(tablelist[table].t[i][index] != r[i])
									indexes = indexes + " " + to_string(i);
							}
							else {
								throw "the strings cannot do other operation than equal";
							}
						}
                        return indexes;
					}
				}
				else{
                    cout << "b is a string" << endl;
                     int index = tablelist[table].get_index(a);
                    if(tablelist[table].attr[a] != "string")
                        throw "the attribute type is not string";
                    string indexes;
                    for(int i =1; i < tablelist[table].t.size(); ++i){
                        if(op == "="){
                            if(tablelist[table].t[i][index] == b){
                                indexes = indexes + " " + to_string(i);
                            }
                        }
                        else if(op == "<>"){
                            if(tablelist[table].t[i][index] != b)
                                indexes = indexes + " " + to_string(i);
                        }
                        else {
                            throw "the strings cannot do other operation than equal";
                        }
                        }return indexes;
                    }
					//throw "it is meaningless to do logical expression between attribute and something other than a number, a string or attribute";
				}
			}
		else if(is_attribute(table,b)){
			//cout << " b is an attribute" << endl;
			if(is_number(a)){
                //cout << "a is a number" << endl;
				int index = tablelist[table].get_index(b);
				//check if the attribute is a number
				if(tablelist[table].attr[b] != "int" && tablelist[table].attr[b] != "double"){
					throw "the type of attribute is incorrect";
				}
                string indexes;
				for(int i = 1; i < tablelist[table].t.size(); ++i){
                    double a1 = atoi(a.c_str());
                    double b1 = atoi(tablelist[table].t[i][index].c_str());
                    
					if(op == "="){
						if(a1 == b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<>"){
						if(a1 != b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == ">"){
						if(a1 > b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<"){
						if(a1 < b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == ">="){
						if(a1 >= b1)
							indexes = indexes + " " + to_string(i);
					}
					else if(op == "<="){
						if(a1 <= b1)
							indexes = indexes + " " + to_string(i);
					}
				}
				return indexes;
                
			}
			
			else{
				//maybe it is a string with the results
				istringstream iss(a);
				vector<string> r;
				for(string s; iss>>s;)
					r.push_back(s);
				
				if(r.size() > 0 && is_attribute(table, r[0])){
                    cout << "a is the data from attribute after arithmetic operation";
					//check if these two attributes has the same type and if they can compare
					if(tablelist[table].attr[b] != tablelist[table].attr[r[0]]){
						throw "the type of attributes are not the same";
					}
					if(tablelist[table].t.size() != r.size()){
                        throw "the size of these two attribute are not the same";
                    }
					
                    int index = tablelist[table].get_index(b);
					//if these two attributes are number
					if(tablelist[table].attr[b] == "int" || tablelist[table].attr[b] == "double"){
						string indexes;
						for(int i = 1; i < r.size(); ++i){
                            double a1 = atoi(tablelist[table].t[i][index].c_str());
                            double b1 = atoi(b.c_str());
							if(op == "="){
								if(a1 == b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<>"){
								if(a1 != b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == ">"){
								if(a1 > b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<"){
								if(a1 < b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == ">="){
								if(a1 >= b1)
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<="){
								if(a1 <= b1)
									indexes = indexes + " " + to_string(i);
							}
						}
                        return indexes;
					}
					
					//if these two attribute are strings
					else{
						string indexes;
						for(int i = 1; i < r.size(); ++i){
							if(op == "="){
								if(r[i] == tablelist[table].t[i][index])
									indexes = indexes + " " + to_string(i);
							}
							else if(op == "<>"){
								if(r[i] != tablelist[table].t[i][index])
									indexes = indexes + " " + to_string(i);
							}
							else {
								throw "the strings cannot do other operation than equality";
							}
						}
                        return indexes;
					}
				}
				else{
                    //cout << "a is a string" << endl;
                     int index = tablelist[table].get_index(b);
                    if(tablelist[table].attr[b] != "string")
                        throw "the attribute type is not string";
                    string indexes;
                    for(int i =1; i < tablelist[table].t.size(); ++i){
                        if(op == "="){
                            if(tablelist[table].t[i][index] == a){
                                indexes = indexes + " " + to_string(i);
                            }
                        }
                        else if(op == "<>"){
                            if(tablelist[table].t[i][index] != a)
                                indexes = indexes + " " + to_string(i);
                        }
                        else {
                            throw "the strings cannot do other operation than equal";
                        }
                        }return indexes;
				}
			}
	
		}
		else{
			throw "logical expression has to contain at least one attribute";
			return "";
		}
	}
	
	int precedence(string op)
	{
		if(op == "*" || op == "/")
			return 3;
		else if(op == "+" || op == "-")
			return 2;
		else if(op == "=" || op == "<>" || op == ">" || op == "<" || op == ">=" || op == "<=")
			return 1;
		return -1;
	}
	
    int logic_precedence(string op)
    {
        if(op == "NOT")
            return 3;
        else if(op == "AND")
            return 2;
        else if(op == "OR")
            return 1;
        return -1;
    }
    
	bool is_number(string& s)
	{
        try
        {
            std::stod(s);
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
	
	bool is_operator(string op)
	{
		return op == "*" || op == "/" || op == "+" || op == "-" || 
		op == "=" || op == "<>" || op == ">" || op == "<" || op == ">=" || op == "<=";
	}
	
	bool is_attribute(string table, string attr)
	{
		return tablelist[table].get_index(attr) != -1;
	}
    
    bool is_logic(string op)
    {
        return op == "AND" || op == "OR" || op == "NOT";
    }

};
