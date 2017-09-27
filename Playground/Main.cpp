#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

string password = "test";
int main() {
	

	cout << "Pass is in " << &password << endl;

	cin.get();

	return 0;
}
