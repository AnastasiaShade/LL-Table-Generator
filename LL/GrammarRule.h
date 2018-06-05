#pragma once

struct Rule
{
	struct RightPart
	{
		using Items = std::vector<std::string>;
		using GuidesSet = std::unordered_set<std::string>;

		Items items;
		GuidesSet guides; // ������������ (������������ ���������)
	};

	using RightParts = std::vector<RightPart>;

	std::string leftPart;
	RightParts rightParts;
};
