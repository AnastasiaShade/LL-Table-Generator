#include "stdafx.h"
#include "Grammar.h"
#include "Table.h"

using namespace std;

void PrintGrammar(const CGrammar::Grammar& grammar);

int main()
{
	string inputFile;

	cout << "Input file: ";
	cin >> inputFile;
	cout << endl;

	/*
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
	*/
	std::ifstream input(inputFile);
	CGrammar grammar;
	grammar.ReadAndParseGrammar(input);
	grammar.ConvertToLL();

	PrintGrammar(grammar.GetGrammar());

	CTable table;

	/* initialization */
	for (const auto& rule : grammar.GetGrammar())
	{
		Rule::RightParts rightParts = rule.rightParts;

		for (const auto& part : rightParts)
		{
			// ��� �� ����� �� �� ������, ������� ������� � ������ ������
			TableRow row;
			row.referencingSet = part.referencingSet;

			//���� ��� ��������� �����,�� ������ ������
			if (&part == &rightParts.back())
			{
				row.isError = true;
			}

			//����� ������ �� items ������ ��������� � ��� ���������

			table.Add(row);
		}
	}
	return 0;
}

void PrintGrammar(const CGrammar::Grammar& grammar)
{
	for (auto items : grammar)
	{
		cout << items.leftPart << ":" << endl;
		for (const auto& line : items.rightParts)
		{
			for (string item : line)
			{
				cout << item << " ";
			}
			cout << endl;
		}
		cout << "------------------------" << endl;
	}
}
