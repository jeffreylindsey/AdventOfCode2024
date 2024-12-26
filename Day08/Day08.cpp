#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day08
{


constexpr std::string_view DayString = "Day08";

/*****************************************************************************/

struct s_Position
{
	//--- Members -------------------------------------------------------------

	int x = 0;
	int y = 0;

	//--- Methods -------------------------------------------------------------

	constexpr auto operator<=>(const s_Position&) const noexcept = default;
};

/*****************************************************************************/

struct s_Map
{
	//--- Members -------------------------------------------------------------

	int Width = 0;
	int Height = 0;

	// Maps frequency to positions.
	std::multimap<char, s_Position> Antennas;

	//--- Methods -------------------------------------------------------------

	bool ContainsPosition(const s_Position& Position) const;
};

/*===========================================================================*/
bool s_Map::ContainsPosition(const s_Position& Position) const
{
	return Position.x >= 0 && Position.x < Width
		&& Position.y >= 0 && Position.y < Height;
}

/*****************************************************************************/

/*===========================================================================*/
s_Map LoadInput(std::ifstream Input)
{
	s_Map Map = {};

	int y = 0;
	for (const std::string& Line : c_LineReader(Input))
	{
		int x = 0;
		for (const char Char : Line)
		{
			if (Char != '.')
				Map.Antennas.insert(std::pair(Char, s_Position{x, y}));

			++x;
		}
		Map.Width = x;

		++y;
	}
	Map.Height = y;

	return Map;
}

/*===========================================================================*/
auto GetAllUniquePairs(const auto View)
{
	using t_Value = std::ranges::range_value_t<decltype(View)>;

	std::vector<std::pair<t_Value, t_Value>> Pairs;

	for (auto LeftIter = View.begin(); LeftIter != View.end(); ++LeftIter)
	{
		for (auto RightIter = std::next(LeftIter);
			RightIter != View.end();
			++RightIter
		)
		{
			Pairs.push_back(std::pair(*LeftIter, *RightIter));
		}
	}

	return Pairs;
}

/*===========================================================================*/
std::pair<s_Position, s_Position> CalcAntinodePositions
( const std::pair<s_Position, s_Position>& AntennaPair
)
{
	const int dx = AntennaPair.second.x - AntennaPair.first.x;
	const int dy = AntennaPair.second.y - AntennaPair.first.y;

	return
		{ s_Position{AntennaPair.first.x - dx, AntennaPair.first.y - dy}
		, s_Position{AntennaPair.second.x + dx, AntennaPair.second.y + dy}
		};
}

/*===========================================================================*/
std::vector<s_Position> CalcAllAntinodePositionsInMap
( const std::pair<s_Position, s_Position>& AntennaPair
, const s_Map& Map
)
{
	std::vector<s_Position> Result;

	const int dx = AntennaPair.second.x - AntennaPair.first.x;
	const int dy = AntennaPair.second.y - AntennaPair.first.y;

	// Negative
	{
		s_Position Position = AntennaPair.first;

		while (Map.ContainsPosition(Position))
		{
			Result.push_back(Position);

			Position.x -= dx;
			Position.y -= dy;
		}
	}

	// Positive
	{
		s_Position Position = AntennaPair.second;

		while (Map.ContainsPosition(Position))
		{
			Result.push_back(Position);

			Position.x += dx;
			Position.y += dy;
		}
	}

	return Result;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		const s_Map Map = LoadInput(std::move(Input));

		std::set<s_Position> AntinodePositions;

		auto Iter = Map.Antennas.begin();
		while (Iter != Map.Antennas.end())
		{
			const char Frequency = Iter->first;

			const auto [FrequencyBeginIter, FrequencyEndIter]
				= Map.Antennas.equal_range(Frequency);

			const auto FrequencyPositions
				= std::ranges::subrange(FrequencyBeginIter, FrequencyEndIter)
					| std::views::values;

			const std::vector<std::pair<s_Position, s_Position>> Pairs
				= GetAllUniquePairs(FrequencyPositions);

			for (const std::pair<s_Position, s_Position>& AntennaPair : Pairs)
			{
				const std::pair<s_Position, s_Position> Antinodes
					= CalcAntinodePositions(AntennaPair);

				if (Map.ContainsPosition(Antinodes.first))
					AntinodePositions.insert(Antinodes.first);

				if (Map.ContainsPosition(Antinodes.second))
					AntinodePositions.insert(Antinodes.second);
			}

			Iter = FrequencyEndIter;
		}

		return static_cast<int>(AntinodePositions.size());
	}

	public:
		AOC_TEST(Sample, 14)
		AOC_TEST(Input, 276)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		const s_Map Map = LoadInput(std::move(Input));

		std::set<s_Position> AllAntinodePositions;

		auto Iter = Map.Antennas.begin();
		while (Iter != Map.Antennas.end())
		{
			const char Frequency = Iter->first;

			const auto [FrequencyBeginIter, FrequencyEndIter]
				= Map.Antennas.equal_range(Frequency);

			const auto FrequencyPositions
				= std::ranges::subrange(FrequencyBeginIter, FrequencyEndIter)
					| std::views::values;

			const std::vector<std::pair<s_Position, s_Position>> Pairs
				= GetAllUniquePairs(FrequencyPositions);

			for (const std::pair<s_Position, s_Position>& AntennaPair : Pairs)
			{
				const std::vector<s_Position> CalculatedPositions
					= CalcAllAntinodePositionsInMap(AntennaPair, Map);

				AllAntinodePositions.insert
					( CalculatedPositions.begin()
					, CalculatedPositions.end()
					);
			}

			Iter = FrequencyEndIter;
		}

		return static_cast<int>(AllAntinodePositions.size());
	}

	public:
		AOC_TEST(Sample, 34)
		AOC_TEST(Input, 991)
};

/*****************************************************************************/


}
