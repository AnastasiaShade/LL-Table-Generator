#include "stdafx.h"
#include "Grammar.h"
#include "Table.h"
#include "TableGenerator.h"

using namespace std;

void PrintGrammar(const CGrammar::Grammar& grammar);
void PrintTable(CTable& table);
void ParseString(const std::string& str, CTable& table);

int main()
{
	string inputFile;

	cout << "Input file: ";
	getline(cin, inputFile);
	cout << endl;

	std::ifstream input(inputFile);
	CGrammar grammar;
	grammar.ReadAndParseGrammar(input);
	//grammar.ConvertToLL();
	PrintGrammar(grammar.GetGrammar());

	CTableGenerator tableGenerator;
	tableGenerator.Generate(grammar.GetGrammar());

	CTable table = tableGenerator.Get();
	PrintTable(table);

	string tmp;
	cout << "\n\nEnter phrase for check\n";
	getline(cin, tmp);
	cout << endl;
	ParseString(tmp, table);

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
	cout << setw(4) << left << "id"
		 << "| "
		 << setw(30) << "guides set"
		 << "| "
		 << setw(7) << "next"
		 << "| "
		 << setw(9) << "isShift"
		 << "| "
		 << setw(11) << "idAtStack"
		 << "| "
		 << setw(9) << "isError"
		 << "| "
		 << setw(7) << "isEnd"
		 << "| "
		 << endl;

	cout << string(90, '-') << endl;

	for (size_t i = 0; i < table.Size(); ++i)
	{
		TableRow currentRow = table.Get(i);
		string set = "{ ";
		size_t counter = 0;
		for (const auto& reference : currentRow.referencingSet)
		{
			set.append(reference).append(" ");
			++counter;
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

void ParseString(const std::string& str, CTable& table)
{
	stringstream strm(str);

	string elem;
	size_t index = 0;
	bool isError = false;
	bool isEnd = false;
	stack<size_t> stack;

	while ((!isError) && (!isEnd))
	{
		strm >> elem;
		cout << "search for " << elem << endl;
		TableRow& row = table.Get(index);

		if (row.isEnd)
		{
			isEnd = true;
			break;
		}

		while (!row.isShift)
		{
			row = table.Get(index);

			cout << index << endl;

			if (row.isEnd)
			{
				isEnd = true;
				break;
			}

			if (row.referencingSet.find(elem) != row.referencingSet.end())
			{
				if (row.idAtStack != boost::none)
				{
					stack.push(row.idAtStack.get());
				}

				if (row.next != boost::none)
				{
					index = row.next.get();
				}
				else
				{
					if (!row.isError)
					{
						++index;
					}
					else
					{
						if (stack.empty())
						{
							isError = true;
							break;
						}
						else
						{
							index = stack.top();
							stack.pop();
						}
					}
				}
			}
			else
			{
				if (row.isError)
				{
					isError = true;
					break;
				}
				else
				{
					++index;
				}
			}
		}
	}

	if (isError)
	{
		cout << "Error" << endl;
	}

	if (isEnd)
	{
		cout << "End" << endl;
	}
}
