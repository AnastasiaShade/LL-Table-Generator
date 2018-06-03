#include "stdafx.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

using namespace std;

struct Rule
{
	string leftPart = "";
	vector<vector<string>> rightParts;
};

void ParseLine(vector<string> & arr, const string & line)
{
	boost::algorithm::split_regex(arr, line, boost::regex("->|( +)"));
}

size_t FindRule(const vector<Rule> & grammar, const string & leftGrammarPart)
{
	for (size_t i = 0; i < grammar.size(); ++i)
	{
		if (grammar[i].leftPart == leftGrammarPart)
		{
			return i;
		}
	}

	return -1;
}

void ReadAndParseGrammar(vector<Rule> & grammar, const string & inputFile)
{
	ifstream input(inputFile);
	string line, leftGrammarPart;
	vector<string> parsedLine;
	while (getline(input, line))
	{
		ParseLine(parsedLine, line);
		leftGrammarPart = parsedLine[0];
		parsedLine.erase(parsedLine.begin());
		size_t pos = FindRule(grammar, leftGrammarPart);
		
		if (pos == -1)
		{
			Rule newRule;
			newRule.leftPart = leftGrammarPart;
			newRule.rightParts.push_back(parsedLine);
			grammar.push_back(newRule);
		}
		else
		{
			grammar[pos].rightParts.push_back(parsedLine);
		}
	}
}

bool AreEquals(const vector<vector<string>> & items, size_t currPos)
{
	string item = items[0][currPos];
	for (size_t i = 1; i < items.size(); ++i)
	{
		if (items[i][currPos] != item)
		{
			return false;
		}
	}

	return true;
}

void ConvertToLL(vector<Rule> & grammar)
{
	for (Rule rule : grammar)
	{
		vector<vector<string>> tmp = rule.rightParts;
		int counter = 1;
		while (!tmp.empty())
		{
			//FindEqual()
			vector<string> curr = *tmp.begin();
			tmp.erase(tmp.begin());
			vector<vector<string>> equals;
			equals.push_back(curr);
			
			for (size_t j = 0; j < tmp.size(); )
			{
				if (curr[0] == tmp[j][0])
				{
					equals.push_back(tmp[j]);
					tmp.erase(tmp.begin() + j);
				}
				else
				{
					++j;
				}
			}


			//if (equals.size() > 1)
			//SetNewRule()
			Rule newRule;
			string leftPart = rule.leftPart;
			leftPart.insert(leftPart.size() - 2, to_string(counter));
			newRule.leftPart = leftPart;


			//GetMinSize()
			size_t minSize = equals[0].size();
			for (auto item : equals)
			{
				if (item.size() > minSize)
				{
					minSize = item.size();
				}
			}


			size_t currPos = 1;
			while (AreEquals(equals, currPos))
			{
				++currPos;
			}



			++counter;
		}
	}
}

int main()
{
	string inputFile;

	cout << "Input file: ";
	cin >> inputFile;
	cout << endl;

	vector<Rule> grammar;
	ReadAndParseGrammar(grammar, inputFile);
	ConvertToLL(grammar);

	for (auto items : grammar)
	{
		cout << items.leftPart << endl;
		for (auto line : items.rightParts)
		{
			for (string item : line)
			{
				cout << item << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	map<string, vector<string>> mainTable;

    return 0;
}

