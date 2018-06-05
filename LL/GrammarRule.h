#pragma once
#include <string>
#include <vector>

struct Rule
{
	using RightPart = std::vector<std::string>;
	using RightParts = std::vector<RightPart>;

	std::string leftPart;
	RightParts rightParts;
};
