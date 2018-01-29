#include "table.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;
int main()
{
	Database d();//output successful
	Table t1();//output successful
	Table t2();//output successful
	
    d.addTable(t1,"abc");//output successful
    d.addTable(t1,"cde");//output failed since we cannot add same table
    d.addTable(t2,"abc");//output failed since we cannot add two tables with same name
    d.addTable(t2,"cde");//output successful

    d.listTables();//print abc and jbc.
    d.getTables();//output all the tables t1 and t2.
    d.findTable("abc")//output the address of t1 and t2.
    d.query()

    d.rmTable("abc");//output successful since we already add a table with name adc
    d.rmTable("cde");//output successful since we already add a table with name cde
    d.rmTable("jz");//output fail since we don't have a table with name jz

    d.listTables();//we don't have table in the database, we should print empty database.
    d.getTables();//we don't have table in the database, we should print empty database.
    d.findTable("abc")//output this table doesn't exist.
    d.query()





}
