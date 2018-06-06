#pragma once
#include "TableRow.h"

class CTable
{
public:
	void Add(const TableRow&);
	size_t Size() const;
	TableRow& Get(size_t);

private:
	std::vector<TableRow> m_rows;
};
