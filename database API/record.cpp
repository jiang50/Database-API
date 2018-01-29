#include "Header.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

int main() 
{
    	record r0; //output successful
	r0.set(1,"lss") //output failed, since r does not have size
	r0.set("qwe","qsd"); //output successful, since r can be set to a record with any size
	r0.set("asdf","dsfa","sdfa") //output successful, since the value of r is updated
	cout << r0.getSize() << endl; //output -1, since its size is not defined

	
	record r1(2);   //output successful
	record r2(0);   //output fail, since the size needs to be larger than 0
	record r3(-2); //output fail, since the size needs to be larger than 0
		
	r1.set(-1,"hahaha");  //output fails, out of bounds
	r1.set(4,"what");  //output fail, out of bounds
	r1.set(0, "23.2");  //output successful
	
	record r4(1);
	r4.set("sdfsd","sdfsdf","sdfsdf"); //output failed, since r4 has a size of 1
	r4.set("haha") //output successful
	
	record r6(100); //output successful
	
	r6[5] = "sdf";  //output failed, since users cannot change values through [] operation
	r6.set(2,"@3");  //output successul
	string s = r6[2]; //assign "@3" to the string s
	cout << r6.getSize() << endl; //output 100
	
}
