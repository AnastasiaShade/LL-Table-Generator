#pragma once
#include "TableRow.h"

class CTable
{
public:
	void Add(const TableRow&);
	TableRow& Get(size_t);

private:
	std::vector<TableRow> m_table;
};
