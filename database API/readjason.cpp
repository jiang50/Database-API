#include <iostream>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "database.h"
#include <string>
#include <unordered_set>
using namespace std;
using json = nlohmann::json;

void process_other()
{
    while(true){
        cout << "We select 1000 records and generate some statistics about the data." << endl;
        cout << "1. statistics between stars level and number of business" << endl;
        cout << "2. statistics between review counts and number of business" << endl;
        cout << "3. statistics between review counts and number of user" << endl;
        cout << "press any other key to return the main menu" << endl;
        
        int i;
        cin >> i;
        
        if(i == 1){
            cout << "star level | number of business" << endl;
            cout << "< = 1      |     16" << endl
                 << "1 ~ 2      |     81" << endl
                 << "2 ~ 3      |     246" << endl
                 << "3 ~ 4      |     373" << endl
                 << "4 ~ 5      |     282" << endl;
        }
        else if(i == 2){
            cout << "review counts | number of business" << endl;
            cout << "<= 5          |    334"  << endl
                 << "5 ~ 10        |    205" << endl
                 << "10 ~ 20       |    176" << endl
                 << "20 ~ 50       |    155" << endl
                 << "> = 50        |    130" << endl;
        }
        else if(i == 3){
            cout << "review counts | number of users" << endl;
            cout << "< = 10        |   199" << endl
                 << "10 ~ 25       |   170" << endl
                 << "25 ~ 50       |   156" << endl
                 << "50 ~ 100      |   143" << endl
                 << "100 ~ 200     |   128" << endl
                 << "200 ~ 500     |   147" << endl
                 << "> = 500       |   53" << endl;
        }
        else{
            break;
        }
    }
}


int main()
try{
	vector<attribute> v1={attribute("business_id","string"),attribute("name","string"),attribute("address","string"),attribute("city","string"),attribute("state","string"),attribute("postal_code","string"),attribute("average_star","double"),attribute("review_count","int")};
	Table business(v1);
	business.specifyKey("business_id");
	vector<attribute> v2={attribute("user_id","string"),attribute("name","string"),attribute("review_count","int"),attribute("average_stars","double")};
	Table user(v2);
	user.specifyKey("user_id");
	vector<attribute> v3={attribute("review_id","string"),attribute("business_id","string"),attribute("star","double"),attribute("text","string"),attribute("user_id","string")};
	Table review(v3);
	review.specifyKey("review_id");
	unordered_set<string> busid;
	unordered_set<string> usid;
	cout<<"loading data..."<<endl;
	ifstream ib("business.json");
	string busi;
	int b=1000;
	while(b>0&&getline(ib,busi)){
		
		json j;
		stringstream sstr(busi);
		sstr>>j;
		double s= j["stars"];
		stringstream ss;
		ss<<fixed<<setprecision(2)<<s;
		string star=ss.str();
		stringstream ss2;
		ss2<<j["review_count"];
		string count=ss2.str();
		business.insert_record(Record({j["business_id"],j["name"],j["address"],j["city"],j["state"],j["postal_code"],star,count}));
		string bid=j["business_id"];
		busid.insert(bid);
		b--;
	}
	ifstream iu("user.json");
	string use;
	int u=1000;
	while(u>0&&getline(iu,use)){
		json j;
		stringstream sstr(use);
		sstr>>j;
		double s= j["average_stars"];
		stringstream ss;
		ss<<fixed<<setprecision(2)<<s;
		string average=ss.str();
		stringstream ss2;
		ss2<<j["review_count"];
		string count=ss2.str();
		user.insert_record(Record({j["user_id"],j["name"],count,average}));
		string uid=j["user_id"];
		usid.insert(uid);
		u--;
	}
	ifstream ir("review.json");
	string rev;
	int r=1000000;
	while(r>0&&getline(ir,rev)){
		json j;
		stringstream sstr(rev);
		sstr>>j;	
		string bid=j["business_id"];
//		string uid=j["user_id"];
		if(busid.count(bid)){
			double s= j["stars"];
			stringstream ss;
			ss<<fixed<<setprecision(2)<<s;
			string star=ss.str();		
			review.insert_record(Record({j["review_id"],j["business_id"],star,j["text"],j["user_id"]}));

		}
		r--;
	}
	Table review_user = review.natural_join(review,user);
	Table review_busi = review.natural_join(review,business);
//	cout<<"review size: "<<review.get_size()<<endl;
//	cout<<"business size: "<<business.get_size()<<endl;
//	cout<<"user size: "<<user.get_size()<<endl;
//	cout<<"review_user size: "<<review_user.get_size()<<endl;
//	cout<<"review_business size: "<<review_busi.get_size()<<endl;
	// for(string i:review_busi.get_attr()){
		// cout<<i<<endl;
	// }
//	review_busi.print();
	Database d;
	d.add(user,"user table");
	d.add(business,"business table");
	d.add(review,"review table");
	d.add(review_user,"review table with user information");
	d.add(review_busi,"review table with business information");
//	Table t = d.query_process({"name","star"},"review table with business information","review_count > 100");
//	t.print();
//	Table query = d.query_process({"name","city","star","address","state","review_count"},"business table","city = Las!Vegas AND star >= 4.5 AND review_count >= 100");
//	cout<<"result size "<<query.get_size()<<endl;
	cout<<"Choose the query service:\n";
	cout<<"1. Display information about an user\n";
	cout<<"2. Display inforamtion about a business\n";
	cout<<"3. Filter business by city and star\n";
	cout<<"4. Display the reviews for a business\n";
	cout<<"5. Other" << endl;
	cout<<"Enter 0 to quit.\n";
	int i;
	while(cin>>i){
	if(i==0)break;
	else if(i==1){
		cout<<"Enter the user's first name or user id: "<<endl;
		string s;
		cin>>s;
		string where="name = "+s+" OR user_id = "+ s;
//		cout<<where<<endl;
		Table query = d.query_process({"user_id","name","review_count","average_stars"},"user table",where);
		vector<Record> re=query.iterate();
		for(Record r:re){
			cout<<"User ID: "<<r.get(0)<<endl;
			cout<<"Name: "<<r.get(1)<<endl;
			cout<<"The number of reviews: "<<r.get(2)<<endl;
			cout<<"The average star is: "<<r.get(3)<<endl;
			cout<<endl;
		}
	}
	else if (i==2){
		cout<<"Enter the business name or business id: "<<endl;
		string s;
		cin.ignore();
		getline(cin,s);
		for(int i=0;i<s.size();i++){
			if(s[i]==' ')s[i]='!';
		}
//		cout<<s<<endl;
		string where="name = "+s+" OR business_id = "+ s;
//		cout<<where<<endl;
		Table query = d.query_process({"business_id","name","address","city","state","postal_code","star","review_count"},"business table",where);
		vector<Record> re=query.iterate();
		for(Record r:re){
			cout<<"Business ID: "<<r.get(0)<<endl;
			cout<<"Name: "<<r.get(1)<<endl;
			cout<<"Address: "<<r.get(2)<<endl;
			cout<<"City: "<<r.get(3)<<endl;
			cout<<"State: "<<r.get(4)<<endl;
			cout<<"Postal_code: "<<r.get(5)<<endl;
			cout<<"Star: "<<r.get(6)<<endl;
			cout<<"The number of reviews: "<<r.get(7)<<endl;
		}
		
	}
	else if(i==3){
		cout<<"Enter the city: "<<endl;
		string city;
		cin.ignore();
		getline(cin,city);
		for(int i=0;i<city.size();i++){
			if(city[i]==' ')city[i]='!';
		}
		cout<<"Enter the minimum star(0-5): "<<endl;
		string st;
		cin>>st;
		string where= "city = "+city+" AND star >= "+st;
		Table query = d.query_process({"business_id","name","address","city","state","postal_code","star","review_count"},"business table",where);
		vector<Record> re=query.iterate();
		for(Record r:re){
			cout<<"Business ID: "<<r.get(0)<<endl;
			cout<<"Name: "<<r.get(1)<<endl;
			cout<<"Address: "<<r.get(2)<<endl;
			cout<<"City: "<<r.get(3)<<endl;
			cout<<"State: "<<r.get(4)<<endl;
			cout<<"Postal_code: "<<r.get(5)<<endl;
			cout<<"Star: "<<r.get(6)<<endl;
			cout<<"The number of reviews: "<<r.get(7)<<endl;
			cout<<endl;
		}
	}
	else if(i==4){
		cout<<"Enter the business id or business name: "<<endl;
		string s;
		cin.ignore();
		getline(cin,s);
		for(int i=0;i<s.size();i++){
			if(s[i]==' ')s[i]='!';
		}
		string where="name = "+s+" OR business_id = "+ s;
		cout<<where<<endl;
		Table query = d.query_process({"text"},"review table with business information",where);
		query.print();
		vector<Record> re=query.iterate();
		for(Record r:re){
			cout<<r.get(0)<<endl;
		}
	}
	else if(i==5){
		process_other ();
	}
	cout<<endl;
	cout<<"Choose the query service:\n";
	cout<<"1. Display information about an user\n";
	cout<<"2. Display inforamtion about a business\n";
	cout<<"3. Filter business by city and star\n";
	cout<<"4. Display the reviews for a business\n";
	cout << "5. display user info" << endl;
	cout<<"Enter 0 to quit.\n";
	cout<<endl;
	}
}
catch(const char* a){
	cout << a << endl;
}

