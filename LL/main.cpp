#include "stdafx.h"
#include "Grammar.h"
#include "Table.h"
#include "TableGenerator.h"

using namespace std;

void PrintGrammar(const CGrammar::Grammar& grammar);
void PrintTable(CTable& table);

int main()
{
	string inputFile;

	cout << "Input file: ";
	cin >> inputFile;
	cout << endl;

	std::ifstream input(inputFile);
	CGrammar grammar;
	grammar.ReadAndParseGrammar(input);
	//grammar.ConvertToLL();
	PrintGrammar(grammar.GetGrammar());

	CTableGenerator tableGenerator;
	tableGenerator.Generate(grammar.GetGrammar());

	PrintTable(tableGenerator.Get());

	return EXIT_SUCCESS;
}

void PrintGrammar(const CGrammar::Grammar& grammar)
{
	for (const auto& items : grammar)
	{
		cout << items.leftPart << ":" << endl;
		for (const auto& part : items.rightParts)
		{
			for (string item : part.items)
			{
				cout << item << " ";
			}
			cout << "/ ";
			for (string item : part.guides)
			{
				cout << item << " ";
			}
			cout << endl;
		}
		cout << "------------------------" << endl;
	}
}

void PrintTable(CTable& table)
{
	cout << setw(4) << left << "id" << "| "
		 << setw(30) << "guides set" << "| "
		 << setw(7) << "next" << "| "
		 << setw(9) << "isShift" << "| "
		 << setw(11) << "idAtStack" << "| "
		 << setw(9) << "isError" << "| "
		 << setw(7) << "isEnd" << "| "
		 << endl;

	cout << string(90, '-') << endl;

	for (size_t i = 0; i < table.Size(); ++i)
	{
		TableRow currentRow = table.Get(i);
		string set = "{ ";
		for (const auto& reference : currentRow.referencingSet)
		{
			set.append(reference).append(", ");
		}
		set.append("}");

		cout << setw(4) << left << (" " + to_string(i)) << "| "
			 << setw(30) << set << "| "
			 << setw(7) << (currentRow.next == boost::none ? "-" : to_string(currentRow.next.get())) << "| "
			 << setw(9) << (currentRow.isShift ? "+" : "-") << "| "
			 << setw(11) << (currentRow.idAtStack == boost::none ? "-" : to_string(currentRow.idAtStack.get())) << "| "
			 << setw(9) << (currentRow.isError ? "+" : "-") << "| "
			 << setw(7) << (currentRow.isEnd ? "+" : "-") << "| "
			 << endl;
	}
}
