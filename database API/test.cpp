#include "database.h"
#include "assert.h"
#include <iostream>
//#include "table.h"


	/* DATABASE TESTS */

	void test_add() {
		cout << "database add test ";
		try {
		Database db;

		/* Test adding of 1 table */
		db.add(Table(), "testTable");

		assert(db.get_alltable().size() == 1);
		assert(db.get_alltableName()[0] == "testTable");


		/* Test adding of second table */
		db.add(Table(), "testTable2");

		assert(db.get_alltable().size() == 2);
		assert(db.get_alltableName()[0] == "testTable2");
		assert(db.get_alltableName()[1] == "testTable");


		/* Test adding table name that already exists */
		db.add(Table(), "testTable");

		assert(db.get_alltable().size() == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_drop() {
		cout << "database drop test ";
		try {
		Database db;

		db.add(Table(), "testTable");
		db.add(Table(), "testTable2");


		/* Test dropping 1 of 2 tables */
		db.drop("testTable");

		assert(db.get_alltable().size() == 1);
		assert(db.get_alltableName()[0] == "testTable2");


		/* Test dropping second table, emptying database */
		db.drop("testTable2");

		assert(db.get_alltable().size() == 0);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_get_alltable() {
		cout << "database get all table test ";
		try {
		Database db;

		/* Test when no tables exist */
		assert(db.get_alltable().size() == 0);

		/* Test when one table exists */
		db.add(Table(), "testTable");

		assert(db.get_alltable().size() == 1);

		/* Test when multiple tables exist */
		db.add(Table(), "testTable2");

		assert(db.get_alltable().size() == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;

	}

	void test_get_alltablename() {
		cout << "database get all table name test ";
		try {
		Database db;

		/* Test when no tables exist */
		assert(db.get_alltableName().size() == 0);

		/* Test when one table exists */
		db.add(Table(), "testTable");

		assert(db.get_alltableName().size() == 1);

		/* Test when multiple tables exist */
		db.add(Table(), "testTable2");

		assert(db.get_alltableName().size() == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_query_process() {
		cout << "database query test ";

		try {

			Database db;
			Table t;

			Table table;
			attribute Attr1("Attr1","string");
			attribute Attr2("Attr2","string");
			attribute Attr3("Attr3","string");
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);
			table.AddAttribute(Attr3);

			db.add(table, "TABLE");
			vector<string> attrs;
			attrs.push_back("Attr1");

			vector<string> records;
			records.push_back("rec1");
			records.push_back("rec2");
			records.push_back("rec3");
			Record r(records);

			table.insert_record(r);

			Table t2 = db.query_process(attrs, "TABLE", "Attr1=rec1");

			assert(t2.get_attr().size() == 1);
			assert(t2.get_size() == 1);

		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}


	/* TABLE TESTS */

	void test_AddAttribute() {
		cout << "table add attribute test ";
		try {
		
		Table table;
		attribute Attr1("Attr1","int");
		table.AddAttribute(Attr1);
		assert(table.get_attr().size() == 1);
		attribute Attr2("Attr2","double");
		table.AddAttribute(Attr2);

		assert(table.get_attr().size() == 2);
		attribute Attr3("Attr3","string");
		table.AddAttribute(Attr3);

		assert(table.get_attr().size() == 3);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_DeleteAttribute() {
		cout << "table delete test ";
		try {
		Table table;
		attribute Attr1("Attr1","int");
		table.AddAttribute(Attr1);
		attribute Attr2("Attr2","double");
		table.AddAttribute(Attr2);

		table.DeleteAttribute("Attr1");

		assert(table.get_attr().size() == 1);

		table.DeleteAttribute("Attr2");

		assert(table.get_attr().size() == 0);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_insert_record() {
		cout << "table insert test ";
		try {
		//Do not know how to test this one

		Table table;
		attribute Attr1("Attr1","string");
		table.AddAttribute(Attr1);

		table.insert_record(Record(1));

		assert(table.get_size() == 1);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_get_attr() {
		cout << "table get attr test ";
		try {
		Table table;
		attribute Attr1("Attr1","int");
		table.AddAttribute(Attr1);

		assert(table.get_attr()[0] == "Attr1");
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_get_size() {
		cout << "table get size test ";
		try {
		Table table;
		attribute Attr1("Attr1","string");
		table.AddAttribute(Attr1);

		table.insert_record(Record(1));

		assert(table.get_size() == 1);

		table.insert_record(Record(1));

		assert(table.get_size() == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_iterate() {
		//DON'T KNOW HOW TO TEST
		cout << "table iterator test ";

		try {
			Table table;
			attribute Attr1("Attr1","int");
			table.AddAttribute(Attr1);		
			table.insert_record(Record ({"1"}));
			assert(table.iterate()[0].get(0)=="1");

		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_cross_join() {
		cout << "table cross join test ";

		try {
			Table table;
			attribute Attr1("Attr1","int");
			attribute Attr2("Attr2","int");
			attribute Attr3("Attr3","int");
			attribute Attr4("Attr4","int");
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);

			Table table2;
			table2.AddAttribute(Attr3);
			table2.AddAttribute(Attr4);

			table.insert_record(Record({"1","2"}));
			table.insert_record(Record({"3","4"}));

			table2.insert_record(Record({"5","6"}));
			table2.insert_record(Record({"7","8"}));

			Table cj = table.cross_join(table, table2);

			assert(cj.get_size() == 4);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_natural_join() {
		cout << "table natural join test ";

		try {
			attribute Attr1("Attr1","int");
			attribute Attr2("Attr2","int");
			attribute Attr3("Attr3","int");
			attribute Attr4("Attr4","int");
			Table table;
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);
			table.AddAttribute(Attr3);

			Table table2;
			table2.AddAttribute(Attr4);
			table2.AddAttribute(Attr3);
			table2.specifyKey("Attr3");
			table.insert_record(Record({"1","2","3"}));
			table.insert_record(Record({"1","2","3"}));

			table2.insert_record(Record({"1","2"}));

			Table nj = table.natural_join(table, table2);

			assert(nj.get_size() == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_count() {
		cout << "table count test ";

		try {
			Table table;
			attribute Attr1("Attr1","int");
			attribute Attr2("Attr2","int");
			attribute Attr3("Attr3","int");
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);
			table.AddAttribute(Attr3);

			table.insert_record(Record({"1","2","3"}));

			assert(table.count("Attr1") == 1);

			table.insert_record(Record({"1","2","3"}));

			assert(table.count("Attr1") == 2);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_Min() {
		cout << "table min test ";

		try {
			Table table;
			attribute Attr1("Attr1","int");
			attribute Attr2("Attr2","int");
			attribute Attr3("Attr3","int");
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);
			table.AddAttribute(Attr3);

			table.insert_record(Record({"1","2","3"}));

			assert(table.Min("Attr1") == "1");

			table.insert_record(Record({"0","2","3"}));

			assert(table.Min("Attr1") == "0");
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_Max() {
		cout << "table max test ";

		try {
			Table table;
			attribute Attr1("Attr1","int");
			attribute Attr2("Attr2","int");
			attribute Attr3("Attr3","int");
			table.AddAttribute(Attr1);
			table.AddAttribute(Attr2);
			table.AddAttribute(Attr3);

			table.insert_record(Record({"1","2","3"}));

			assert(table.Max("Attr1") == "1");

			table.insert_record(Record({"4","2","3"}));

			assert(table.Max("Attr1") == "4");
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;	
	
	}

	/* RECORD TESTS */

	void test_size() {
		cout << "record size test ";
		try {
			Record r(3);

			assert(r.size() == 3);
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;	
	}

	void test_get() {
		cout << "record get test ";
		try {
			vector<string> records;
			records.push_back("rec1");
			records.push_back("rec2");
			Record r(records);

			assert(r.get(0) == "rec1");
			assert(r.get(1) == "rec2");
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

	void test_set() {
		cout << "record set test ";
		try {
			vector<string> records;
			records.push_back("rec1");
			records.push_back("rec2");
			Record r(records);

			r.set(0, "rec1NEW");
			r.set(1, "rec2NEW");

			assert(r.get(0) == "rec1NEW");
			assert(r.get(1) == "rec2NEW");
		} catch(...) {
			cout << "FAILED" << endl;
			return;
		}

		cout << "SUCCEEDED" << endl;
	}

int main() {

		/* DATABASE TESTS */

		test_add();
		test_drop();
		test_get_alltable();
		test_get_alltablename();
		test_query_process();

		/* TABLE TESTS */
		test_AddAttribute();
		test_DeleteAttribute();
		test_insert_record();
		test_get_attr();
		test_get_size();
		test_iterate();

		test_cross_join();
		test_natural_join();
		test_count();
		test_Min();
		test_Max();

		/* RECORD TESTS */

		test_size();
		test_get();
		test_set();

	
}
