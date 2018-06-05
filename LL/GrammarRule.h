#pragma once

struct RightPart
{
	std::vector<std::string> items;
	std::unordered_set<std::string> referencingSet;
};

struct Rule
{
	using RightParts = std::vector<RightPart>;

	std::string leftPart;
	RightParts rightParts;
};
