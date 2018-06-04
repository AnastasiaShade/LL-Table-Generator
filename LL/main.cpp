#include "stdafx.h"
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Rule
{
	string leftPart;
	vector<vector<string>> rightParts;
};

void ParseLine(vector<string>& arr, const string& line)
{
	boost::algorithm::split_regex(arr, line, boost::regex("->|( +)"));
}

/* return index of first rule with leftGrammarPart or -1 if rule not exist */
long GetRuleIndex(const vector<Rule>& grammar, const string& leftGrammarPart)
{
	auto comparator = [&leftGrammarPart](const Rule& rule) {
		return rule.leftPart == leftGrammarPart;
	};
	auto result = std::find_if(grammar.begin(), grammar.end(), comparator);

	return (result != grammar.end()) ? std::distance(grammar.begin(), result) : -1;
}

void ReadAndParseGrammar(vector<Rule>& grammar, const string& inputFile)
{
	ifstream input(inputFile);
	string line, leftGrammarPart;
	vector<string> parsedLine;
	while (getline(input, line))
	{
		ParseLine(parsedLine, line);
		leftGrammarPart = parsedLine[0];
		parsedLine.erase(parsedLine.begin());
		size_t pos = GetRuleIndex(grammar, leftGrammarPart);

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

size_t example;

bool AreEquals(const vector<vector<string>>& items, size_t currPos)
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

void ConvertToLL(vector<Rule>& grammar)
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

			for (size_t j = 0; j < tmp.size();)
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

			if (equals.size() > 1)
			{
				string newleftPart = rule.leftPart;
				newleftPart.insert(newleftPart.size() - 2, to_string(counter));

				size_t minSize = equals[0].size();
				for (auto item : equals)
				{
					if (item.size() < minSize)
					{
						minSize = item.size();
					}
				}

				size_t currPos = 0;
				vector<string> equalRightPart;
				vector<vector<string>> differentRightParts;

				while (currPos < minSize)
				{
					if (AreEquals(equals, currPos))
					{
						equalRightPart.push_back(curr.at(currPos));
						++currPos;
					}
					else
					{
						break;
					}
				}

				for (auto equal : equals)
				{
					if (equal.size() == currPos)
					{
						differentRightParts.emplace_back(vector<string>(1, ""));
					}
					if (equal.size() > currPos)
					{
						vector<string> tmp;
						for (size_t i = currPos; i < equal.size(); ++i)
						{
							tmp.emplace_back(equal.at(i));
						}
						differentRightParts.push_back(tmp);
					}
				}

				rule.rightParts = vector<vector<string>>(1, equalRightPart);

				Rule tmpRule;
				for (auto rightPart : differentRightParts)
				{
					tmpRule.leftPart = newleftPart;
					tmpRule.rightParts = vector<vector<string>>(1, rightPart);
					grammar.emplace_back(tmpRule);
				}
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
