#include <iostream>
using namespace std;

int main() {
	int snumber;
	string operation;
	int fnumber;
	
	cout << "Please enter the first number: ";
	cin >> fnumber;
	
	cout << "Please choose the operation" << endl << " +  -  /  *" << endl;
	cin >> operation;
	
	cout << "Please enter the second number: ";
	cin >> snumber;
	
	if (operation == "+") {
		cout << "Answer: " <<fnumber + snumber << endl;
	}
	
	if (operation == "-") {
		cout << "Answer: " << fnumber - snumber << endl;
	}
	
	if (operation == "/") {
		cout << "Answer: " << fnumber / snumber << endl;
	}
	
	if (operation == "*") {
		cout << "Answer: " << fnumber * snumber << endl;
	}
	return 0;
	
}
	