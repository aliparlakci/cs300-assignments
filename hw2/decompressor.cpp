#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "table.h"

using namespace std;

string getInputString(string filename);
void initializeDict(string table[]);
string decompress(const string& encoded, string * table);
int nextEmpty(string table[]);
void writeOutputString(string data, string filename);

int NOT_FOUND = INT_MIN;

int main()
{
	string input = getInputString("compout.txt");

	string dict[4096] = { "" };

	initializeDict(dict);
	string decoded = decompress(input, dict);

	writeOutputString(decoded, "decompout.txt");

	return 0;
}

string decompress(const string & encoded, string * table)
{
	ostringstream result;
	istringstream input(encoded);

	int counter = 2;

	int code;
	int priorCode = -1;

	while (input >> code)
	{
		if (table[code] != "")
		{
			if (priorCode > 1)
			{
				table[nextEmpty(table)] = table[priorCode] + table[code][0];
			}
			result << table[code];
		}
		else
		{
			table[code] = table[priorCode] + table[priorCode][0];
			result << table[code];
		}

		priorCode = code;
	}

	return result.str();
}

int nextEmpty(string table[])
{
	for (int i = 0; i < 4096; i++)
	{
		if (table[i] == "")
			return i;
	}
}

void initializeDict(string table[])
{
	for (int i = 0; i < 2; i++)
	{
		string temp = "";
		char ch = (char)i;
		temp += ch;

		table[i] = temp;
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
		result += ch;
	}
	input.close();

	return result;
}

void writeOutputString(string data, string filename)
{
	ofstream output;
	output.open(filename);
	output << data;
	output.close();
}