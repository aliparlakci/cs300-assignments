#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "table.h"

using namespace std;

string getInputString(string filename);
void initializeDict(HashTable<string, int>& table);
string compress(string input, HashTable<string, int>& table);
void writeOutputString(string data, string filename);

int NOT_FOUND = INT_MIN;

int main()
{
	string input = getInputString("compin.txt");

	HashTable<string, int> dict(NOT_FOUND);

	initializeDict(dict);
	string encoded = compress(input, dict);

	writeOutputString(encoded, "compout.txt");

	return 0;
}

string compress(string input, HashTable<string, int>& table)
{
	ostringstream result;

	int counter = 256;
	int ptr = 0;

	string s = "";
	char c;

	while (ptr < input.size())
	{
		c = input[ptr];

		if (table.get(s+c) != NOT_FOUND)
		{
			s = s + c;	
		}
		else
		{
			result << table.get(s) << " ";
			table.set(s + c, counter++);
			s = c;
		}

		ptr++;
	}

	if (table.get(s) != NOT_FOUND)
	{
		result << table.get(s);
	}
	
	return result.str();
}

void initializeDict(HashTable<string, int> &table)
{
	for (int i = 0; i < 256; i++)
	{
		string temp = "";
		char ch = (char)i;
		temp += ch;

		table.set(temp, i);
	}
}

string getInputString(string filename)
{
	char ch;
	ifstream input(filename);
	string result = "";

	while (!input.eof())
	{
		input.get(ch);

		if (ch != ' ')
			result += ch;
	}
	input.close();
	
	return result;
}

void writeOutputString(string data, string filename)
{
	ofstream output;
	output.open(filename);
	output << data << " ";
	output.close();
}