#include<bits/stdc++.h>
#include "settings.h"
using namespace std;

int main(int argc , char** argv)
{
	string out = "abc";

	initialize();
	cout << "initialize done ................" << endl;
	set_settings();
	cout << "initialize done ................" << endl;
	opins_setter();
	set_signs();
	cout << "initialize done ................" << endl;
	set_opcodes();
	cout << "initialize done ................" << endl;

	ifstream input = check_file(argc , argv , out);
	cout << "initialize done ................" << endl;

	take_input(input);
	cout << "initialize done ................" << endl;

	syntax_checker();
	cout << "initialissse done ................" << endl;

	write_file(out);
	cout << "initialize done ................" << endl;

	return 0;
}