Table t1();
vector<string> v={"name","age","GPA","UIN"};
Table t2(v);
t1.addAttr("UIN");
t1.addAttr("test score");
Record r(2);
r.set("1234567","90");
t2.addAttr("year");
t2.deleteAttr("age");
Record rc(4);
rc.set(1,"3.5");
rc.set("kl,2.5,1234567,1990");
Record rc2(4);
rc2.set("LZ,3.5,1234597,1999");
t2.insert(rc);
t2.insert(rc2);
vector<string> attr=t2.getAttributes();
cout<<"Attributes: ";
for(string s:attr)cout<<s<<" ";//print attributes in table 2
cout<<endl;
int sz=t2.getSize();
cout<<sz==attr.size();
t2.setKey("UIN");
t2.setName("Students");
int index=t2.findAttrIndex("GPA");
cout<<index<<endl;//1
cout<<t2.recordCount("name")<<endl;//print 2
cout<<t2.recordMin("GPA")<<endl;//print 2.5
cout<<t2.recordMax("GPA")<<endl;//print 3.5
table t3=crossJoin(t2,t1);//then iterate t3 to check if join is correct
table t4(attr);
Record rc3(4);
rc3.set("JY,4.0,3214566,1996");
t4.insert(rc3);
table t5=join(t4,t2);//iterate t5 to check if it is right
table t6=join(t2,t1);//should throw an exception


