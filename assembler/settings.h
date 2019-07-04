#include<bits/stdc++.h>
using namespace std;

bool flag = false;

char rs , ts;

int pc = 0;

vector< vector<string> > ins , parsed , unparsed;
map< vector<string> , vector< vector<string> > > pi;
map<string , vector<string> > opins , pins;
map<string , int> reg;
set<string> regs;
map<string , vector<int> > sign;
map<string , string> opc , linkc;
set<string> op , links , p;

void initialize()
{
	op.insert("set");
	op.insert("add");
	op.insert("sub");
	op.insert("and");
	op.insert("or");
	op.insert("xor");
	op.insert("sll");
	op.insert("srl");
	op.insert("sra");
	op.insert("mul");
	op.insert("div");
	op.insert("slt");
	op.insert("lw");
	op.insert("sw");
	op.insert("beq");
	op.insert("bne");
	op.insert("j");
	op.insert("jal");
	op.insert("jr");
	op.insert("mv");
}

string strip(string s , char c)
{ 
	int i;
	for(i = 0 ; i < s.size() ; ++i)
	{
		if(c == ' ')
		{
			if(s[i] != ' ' and s[i] != '\t')
				break;
		}
		else
		{
			if(s[i] != c)
				break;
		}
	}

	string temp(s,i);

	for(i = temp.size() - 1 ; i >= 0 ; --i)
	{
		if(temp[i] != c)
			break;
	}

	string temp1(temp,0,i + 1);

	return temp1;
}

vector<string> split(string s, char c)
{
	if(s == "")
	{
		vector<string> v;
		return v;
	}

	s = strip(s,c);
	s.push_back(c);

	int i;

	for(i = 0 ; i < s.size() ; ++i)
	{
		if(s[i] == c)
			break;
	}

	string s1(s,0,i);
	string s2(s , i+1);

	vector<string> v = split(s2 , c);
	v.insert(v.begin() , s1);

	return v;
}

void set_settings()
{
	int i = 0;
	ifstream file;
	file.open("settings.st");

	if(!file.is_open())
	{
		cout << "error......" << endl;

		return;
	}

	while(!file.eof())
	{
		string s;
		getline(file , s);

		if(s != "")
			i++;

		if(s != "")
		{
			switch(i)
			{
				case 1:
				{
					rs = s[21];
					break;
				}

				case 2:
				{
					ts = s[21];
					break;
				}

				case 3:
				{
					s = strip(s , ' ');
					string temp(s,21,s.size() - 22);
					temp = strip(temp , ' ');

					vector<string> v = split(temp , '|');

					for(vector<string>::iterator iter = v.begin() ; iter != v.end() ; ++iter)
					{
						temp = strip(*iter , ' ');

						string temp1(temp , 3 , temp.size() - 4);

						reg[temp1] = temp[1] - '0';

						regs.insert(temp1);
					}

					break;
				}

				case 6:
				{
					while(s != "}")
					{
						int i = 0;
						s = strip(s , ' ');

						string temp;

						for(i = 0 ; i < s.size() ; ++i)
						{
							if(s[i] == '-')
								break;

							temp.push_back(s[i]);
						}

						temp = strip(temp , ' ');

						vector<string> v = split(temp , ts);
						p.insert(v[0]);
						pins[v[0]] = v;

						i += 4;

						string inst(s,i,s.size() - i - 1);

						vector<string> ins = split(inst , ',');

						for(vector<string>::iterator iter = ins.begin() ; iter != ins.end() ; ++iter)
						{
							*iter = strip(*iter , ' ');

							if(*iter == ">")
								continue;

							vector<string> instruct = split(*iter , ts);

							pi[v].push_back(instruct);
						}

						getline(file , s);
					}

					break;
				}
			}
		}
	}

	file.close();
}

void opins_setter()
{
	for(map< vector<string> , vector< vector<string> > >::iterator iter = pi.begin() ; iter != pi.end() ; ++iter)
	{
		string s = strip((iter -> first)[0] , ' ');
		opins[s] = (iter -> first);
		op.insert(s);
	}
}

void set_signs()
{
	sign["set"].push_back(0);
	sign["add"].push_back(1);sign["add"].push_back(1);
	sign["sub"].push_back(1);sign["sub"].push_back(1);	
	sign["mul"].push_back(1);sign["mul"].push_back(1);
	sign["div"].push_back(1);sign["div"].push_back(1);
	sign["and"].push_back(1);sign["and"].push_back(1);
	sign["or"].push_back(1);sign["or"].push_back(1);
	sign["xor"].push_back(1);sign["xor"].push_back(1);
	sign["sll"].push_back(1);sign["sll"].push_back(1);
	sign["srl"].push_back(1);sign["srl"].push_back(1);
	sign["sra"].push_back(1);sign["sra"].push_back(1);
	sign["beq"].push_back(1);sign["beq"].push_back(1);sign["beq"].push_back(2);
	sign["bne"].push_back(1);sign["bne"].push_back(1);sign["bne"].push_back(2);
	sign["jr"].push_back(1);sign["jr"].push_back(2);
	sign["jal"].push_back(2);
	sign["j"].push_back(2);
	sign["mv"].push_back(1);sign["mv"].push_back(1);

	for(map<string , vector<string> >::iterator iter = opins.begin() ; iter != opins.end() ; ++iter)
	{
		for(vector<string>::iterator it = ((iter -> second).begin() + 1) ; it != (iter -> second).end() ; ++it)
		{
			string temp(*it , 0 , 3);

			if(temp == "int")
				sign[(iter -> first)].push_back(0);
			if(temp == "add")
				sign[(iter -> first)].push_back(2);
			if(temp == "reg")
				sign[(iter -> first)].push_back(1);
		}
	}
}

string int_to_bi(int n , int len)
{
	string s("");

	while(n)
	{
		s.insert(0,1,(n%2) + '0');
		n /= 2;
	}

	n = s.length();

	while(n < len)
	{
		s.insert(0,1,'0');
		n++;
	}

	return s;
}

void set_opcodes()
{
	int i = 0;
	for(set<string>::iterator iter = op.begin() ; iter != op.end() ; ++iter)
	{
		if(p.find(*iter) == p.end())
		{
			opc[*iter] = int_to_bi(i,8);
			i++;
		}
	}
}

ifstream check_file(int argc , char** inp , string &out)
{
	ifstream input;

	if(argc < 3)
	{
		cout << "Error:  not enough arguments !!!!!!!!!" << endl;
		exit(1);
	}
	else if(argc > 3)
	{
		cout << "Error:  too many input files !!!!!!!!!" << endl;
		exit(1);
	}
	else
	{	
		string fl = inp[1];

		if(fl != "-d" and fl != "-i")
		{
			cout << "Error:  not valid flag !!!!!!!!!" << endl;
			exit(1);
		}
		else
		{
			if(fl == "-i")
				flag = true;
		}

		string s = inp[2];

		string temp(s,0 , s.length() - 2);

		out = temp;

		if(*(s.end() - 1) != 's' or *(s.end() - 2) != '.')
		{
			cout << "Error:  not a assembly file !!!!!!!!!" << endl;
			exit(1);
		}
		else
		{
			input.open(inp[2]);

			if(!input.is_open())
			{
				cout << "Error: unable to open the file ........." << endl;
				exit(1);
			}
		}
	}

	return input;
}

void take_input(ifstream &inp)
{
	while(!inp.eof())
	{
		string s;
		getline(inp,s);

		s = strip(s,' ');

		vector<string> v = split(s,ts);

		if(v.size() > 0)
			ins.push_back(v);
	}
}

int len(int n)
{
	int i = 0;

	if(n==0)
		return 1;

	if(n<0)
		i++;

	while(n)
	{
		n /= 10;
		i++;
	}

	return i;
}

void syntax_checker()
{
	for(vector< vector<string> >::iterator iter = ins.begin() ; iter != ins.end() ; ++iter)
	{
		if(op.find((*iter)[0]) != op.end())
		{
			vector<int> sgn = sign[(*iter)[0]];
			vector<string> tkn;

			if(iter -> size() != sgn.size() + 1)
			{
				cout << "Error: different no. of parameters for " + (*iter)[0] + " ..........." << endl;
				exit(1);
			}

			else
			{
				for(int i = 0 ; i < sgn.size() ; ++i)
				{
					string temp = (*iter)[i+1];

					switch(sgn[i])
					{
						case 0:
						{
							int n = stoi(temp);

							if(len(n) != temp.size())
							{
								cout << "Error: " + temp + " is not an integer ..........." << endl;
								exit(1);
							}
							else
							{
								tkn.push_back(int_to_bi(n,8));
							}

							break;
						}

						case 1:
						{
							if(temp[0] != rs)
							{
								cout << "Error: " << temp[0]  << " is not a register indentifier ..........." << endl;
								exit(1);
							}
							else
							{
								string temp1(temp,1);

								if(regs.find(temp1) == regs.end())
								{
									cout << "Error: " + temp1 + " is not a register ..........." << endl;
									exit(1);
								}
								else
									tkn.push_back(int_to_bi(reg[temp1] , 8));
							}

							break;
						}

						case 2:
						{
							if(links.find(temp) != links.end())
							{
								tkn.push_back(linkc[temp]);
							}
							else
							{
								cout << "Error: " + temp + " is not a link ..........." << endl;
								exit(1);
							}

							break;
						}
					}
				}
			}

			if(p.find((*iter)[0]) != p.end())
			{
				map<string , string> m;
				vector< vector<string> > converted;

				for(int i = 0 ; i < tkn.size() ; ++i)
				{
					m[(pins[(*iter)[0]])[1+i]] = (*iter)[i+1];
				}

				for(vector< vector<string> >::iterator i = (pi[opins[(*iter)[0]]]).begin() ; i != (pi[opins[(*iter)[0]]]).end() ; ++i)
				{
					vector<string> to;
					for(int j = 0 ; j < i -> size() ; ++j)
					{
						if(m[(*i)[j]] == "")
						{
							to.push_back((*i)[j]);
						}
						else
						{
							to.push_back(m[(*i)[j]]);	
						}
					}

					converted.push_back(to);
					unparsed.push_back(to);
				}

				for(vector< vector<string> >::iterator iter = converted.begin() ; iter != converted.end() ; ++iter)
				{
					if(op.find((*iter)[0]) != op.end())
					{
						vector<int> sgn = sign[(*iter)[0]];
						vector<string> tkn;

						if(iter -> size() != sgn.size() + 1)
						{
							cout << "Error: different no. of parameters for " + (*iter)[0] + " ..........." << endl;
						}

						else
						{
							for(int i = 0 ; i < sgn.size() ; ++i)
							{
								string temp = (*iter)[i+1];

								switch(sgn[i])
								{
									case 0:
									{
										int n = stoi(temp);

										if(len(n) != temp.size())
										{
											cout << "Error: " + temp + " is not an integer ..........." << endl;
										}
										else
										{
											tkn.push_back(int_to_bi(n,8));
										}

										break;
									}

									case 1:
									{
										if(temp[0] != rs)
										{
											cout << "Error: " << temp[0]  << " is not a register indentifier ..........." << endl;
										}
										else
										{
											string temp1(temp,1);

											if(regs.find(temp1) == regs.end())
											{
												cout << "Error: " + temp1 + " is not a register ..........." << endl;
											}
											else
												tkn.push_back(int_to_bi(reg[temp1] , 8));
										}

										break;
									}

									case 2:
									{
										if(links.find(temp) != links.end())
										{
											tkn.push_back(linkc[temp]);
										}
										else
										{
											cout << "Error: " + temp + " is not a link ..........." << endl;
										}

										break;
									}
								}
							}
						}
						string val = opc[(*iter)[0]];

						tkn.insert(tkn.begin(),val);
						parsed.push_back(tkn);
						pc++;

					}
					else
					{
						if(iter -> size() == 1)
						{
							links.insert((*iter)[0]);

							bitset<32> b(pc);

							string temp2(b.to_string() , 24);

							linkc[(*iter)[0]] = temp2;
						}
						cout << "Error: " + (*iter)[0] + " is not an instruction ..........." << endl;
					}
				}
			}
			else
			{
				string val = opc[(*iter)[0]];

				tkn.insert(tkn.begin(),val);
				parsed.push_back(tkn);
				pc++;
				unparsed.push_back(*iter);
			}
		}
		else
		{
			if(iter -> size() == 1)
			{
				links.insert((*iter)[0]);

				bitset<32> b(pc);

				string temp2(b.to_string() , 24);

				linkc[(*iter)[0]] = temp2;
				unparsed.push_back(*iter);
			}
			else
				cout << "Error: " + (*iter)[0] + " is not an instruction ..........." << endl;
		}
	}
}

void write_file(string file)
{
		ofstream out;
		out.open(file + ".o");

		for (vector<vector<string> >::iterator i = parsed.begin(); i != parsed.end(); ++i)
		{
			string s = "";
			int count = 0;

			for(vector<string>::iterator iter = i -> begin() ; iter != i -> end() ; ++iter)
			{
				s += (*iter) + " ";
				count++;
			}

			while(count < 4)
			{
				s += "00000000 ";
				count++;
			}

			out << s << endl;
		}

		out.close();

	if(flag)
	{
		ofstream out;
		out.open(file + "_converted.s");

		for (vector<vector<string> >::iterator i = unparsed.begin(); i != unparsed.end(); ++i)
		{
			string s = "";
			for(vector<string>::iterator iter = i -> begin() ; iter != i -> end() ; ++iter)
			{
				s += *iter + " ";	
			}

			out << s << endl;
		}

		out.close();
	}
}