#include "stdafx.h"
#include "Grammar.h"
#include "Table.h"

using namespace std;

void PrintGrammar(const CGrammar::Grammar& grammar);
bool IsTerminal(const std::string& symbol);

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

	CTable table;
	std::vector<Rule::RightPart::Items> unresolvedNextIds;
	std::map<std::string, std::vector<size_t>> tableReferences;
	std::map<std::string, std::unordered_set<std::string>> guidesSet;

	/* initialization */
	size_t currentRowNumber = 0;
	for (const auto& rule : grammar.GetGrammar())
	{
		Rule::RightParts rightParts = rule.rightParts;

		for (const auto& part : rightParts)
		{
			TableRow row;
			row.referencingSet = part.guides;
			//если это последняя часть,то ставим ошибку
			if (&part == &rightParts.back())
			{
				row.isError = true;
			}

			////////////////////
			//конст ссылку на items должны запихнуть в доп структуру
			unresolvedNextIds.emplace_back(part.items);

			auto reference = tableReferences.find(rule.leftPart);
			if (reference == tableReferences.end())
			{
				tableReferences.emplace(rule.leftPart, std::vector<size_t>(1, currentRowNumber));
			}
			else
			{
				reference->second.emplace_back(currentRowNumber);
			}

			auto guide = guidesSet.find(rule.leftPart);
			if (guide == guidesSet.end())
			{
				guidesSet.emplace(rule.leftPart, part.guides);
			}
			else
			{
				for (auto tmp : part.guides)
				{
					if (guide->second.find(tmp) == guide->second.end())
					{
						guide->second.emplace(tmp);
					}
				}
			}

			/////////////////////

			table.Add(row);
			++currentRowNumber;
		}
	}

	size_t nextUnresolvedIndex = 0;
	for (const auto& unresolvedNextId : unresolvedNextIds)
	{
		Rule::RightPart::Items items;

		table.Get(nextUnresolvedIndex).next = currentRowNumber; // берем из table строку с индексом текущего m_unresolvedNextIds и там в next ставим m_currentTableRowId

		for (const auto& item : unresolvedNextId)
		{
			TableRow row;

			if (IsTerminal(item))
			{
				/* у нас лексер еще не прикручен */
				/*
				Token symbolValue;
				if (!TokenExtensions::CreateFromString(symbolValueString, symbolValue))
				{
					throw std::runtime_error("Cannot create token from string: " + symbolValueString);
				}
				*/

				row.referencingSet = Rule::RightPart::GuidesSet({ item });

				if (&item != &unresolvedNextId.back())
				{
					row.next = currentRowNumber;
				}

				if (item == "#")
				{

					row.isEnd = true;
				}
				else
				{
					row.isShift = true;
				}
			}
			else
			{
				row.referencingSet = guidesSet.at(item);

				if (item != "")
				{
					row.next = tableReferences.at(item).front(); //наверно так
				}

				if ((&item != &unresolvedNextId.back()))
				{
					row.idAtStack = currentRowNumber;
				}
			}

			row.isError = true;

			table.Add(row);
			++currentRowNumber;
		}
	}

	return EXIT_SUCCESS;
}

bool IsTerminal(const std::string& symbol)
{
	if (symbol.empty())
	{
		return false;
	}
	return symbol.front() == '<' && symbol.back() == '>';
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
