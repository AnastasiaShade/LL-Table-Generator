#include "stdafx.h"
#include "Table.h"

void CTable::Add(const TableRow& row)
{
	m_table.emplace_back(row);
}

size_t CTable::Size() const
{
	return m_table.size();
}

TableRow& CTable::Get(size_t index)
{
	return m_table.at(index);
}
