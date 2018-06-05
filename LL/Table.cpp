#include "stdafx.h"
#include "Table.h"

void CTable::Add(const TableRow& row)
{
	m_table.emplace_back(row);
}

TableRow& CTable::Get(size_t index)
{
	return m_table.at(index);
}
