#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day10
{


constexpr std::string_view DayString = "Day10";

/*****************************************************************************/
// s_Position

struct s_Position
{
	//--- Members -------------------------------------------------------------

	int x = 0;
	int y = 0;

	//--- Methods -------------------------------------------------------------

	constexpr auto operator<=>(const s_Position&) const noexcept = default;

	s_Position Up() const;
	s_Position Down() const;
	s_Position Left() const;
	s_Position Right() const;
};

/*===========================================================================*/
s_Position s_Position::Up() const
{
	return {x, y - 1};
}

/*===========================================================================*/
s_Position s_Position::Down() const
{
	return {x, y + 1};
}

/*===========================================================================*/
s_Position s_Position::Left() const
{
	return {x - 1, y};
}

/*===========================================================================*/
s_Position s_Position::Right() const
{
	return {x + 1, y};
}

/*****************************************************************************/
// c_TopographicMap

class c_TopographicMap
{
	//--- Public Methods ------------------------------------------------------
	public:
		void AddLine(const std::string_view Line);

		int Width() const;
		int Height() const;

		char GetHeightAt(const s_Position& Position) const;

	//--- Private Members -----------------------------------------------------
	private:
		size_t m_Width = 0;
		std::vector<char> m_Heights;
};

/*===========================================================================*/
void c_TopographicMap::AddLine(const std::string_view Line)
{
	if (m_Width == 0)
		m_Width = Line.length();

	// Assuming all lines in the puzzle are of equal length.
	Assert::AreEqual(m_Width, Line.length());

	m_Heights.insert(m_Heights.end(), Line.begin(), Line.end());
}

/*===========================================================================*/
int c_TopographicMap::Width() const
{
	return static_cast<int>(m_Width);
}

/*===========================================================================*/
int c_TopographicMap::Height() const
{
	if (m_Width == 0)
		return 0;

	return static_cast<int>(m_Heights.size() / m_Width);
}

/*=============================================================================
	Return 0 (NUL) if the given Column or Row are outside of the valid area.
-----------------------------------------------------------------------------*/
char c_TopographicMap::GetHeightAt(const s_Position& Position) const
{
	if (Position.x < 0 || Position.x >= m_Width)
		return 0;

	if (Position.y < 0)
		return 0;

	const size_t Index = Position.y * m_Width + Position.x;

	if (Index >= m_Heights.size())
		return 0;

	return m_Heights[Index];
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	void Explore
	( const c_TopographicMap& Map
	, const s_Position& Position
	, const char PriorHeight
	, std::set<s_Position>& r_Peaks
	)
	{
		const char Height = Map.GetHeightAt(Position);

		if (Height != PriorHeight + 1)
			return;

		if (Height == '9')
		{
			r_Peaks.insert(Position);
			return;
		}

		Explore(Map, Position.Up(), Height, r_Peaks);
		Explore(Map, Position.Down(), Height, r_Peaks);
		Explore(Map, Position.Left(), Height, r_Peaks);
		Explore(Map, Position.Right(), Height, r_Peaks);
	}

	/*=======================================================================*/
	int ScoreTrailhead(const c_TopographicMap& Map, const s_Position& Trailhead)
	{
		std::set<s_Position> Peaks;

		Explore(Map, Trailhead, '0' - 1, Peaks);

		return static_cast<int>(Peaks.size());
	}

	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		c_TopographicMap Map;

		// Load the input.
		for (const std::string& Line : c_LineReader(Input))
			Map.AddLine(Line);

		// Find each trailhead.
		std::vector<s_Position> Trailheads;
		for (int y = 0; y < Map.Height(); ++y)
		{
			for (int x = 0; x < Map.Width(); ++x)
			{
				const s_Position Position{x, y};

				if (Map.GetHeightAt(Position) == '0')
					Trailheads.push_back(Position);
			}
		}

		// Accumulate the scores for each trailhead.
		int SumOfScores = 0;
		for (const s_Position& Trailhead : Trailheads)
			SumOfScores += ScoreTrailhead(Map, Trailhead);

		return SumOfScores;
	}

	public:
		AOC_TEST(Sample, 36)
		AOC_TEST(Input, 709)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Explore
	( const c_TopographicMap& Map
	, const s_Position& Position
	, const char PriorHeight
	)
	{
		const char Height = Map.GetHeightAt(Position);

		if (Height != PriorHeight + 1)
			return 0;

		if (Height == '9')
			return 1;

		return Explore(Map, Position.Up(), Height)
			+ Explore(Map, Position.Down(), Height)
			+ Explore(Map, Position.Left(), Height)
			+ Explore(Map, Position.Right(), Height);
	}

	/*=======================================================================*/
	int RateTrailhead(const c_TopographicMap& Map, const s_Position& Trailhead)
	{
		return Explore(Map, Trailhead, '0' - 1);
	}

	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		c_TopographicMap Map;

		// Load the input.
		for (const std::string& Line : c_LineReader(Input))
			Map.AddLine(Line);

		// Find each trailhead.
		std::vector<s_Position> Trailheads;
		for (int y = 0; y < Map.Height(); ++y)
		{
			for (int x = 0; x < Map.Width(); ++x)
			{
				const s_Position Position{x, y};

				if (Map.GetHeightAt(Position) == '0')
					Trailheads.push_back(Position);
			}
		}

		// Accumulate the ratings for each trailhead.
		int SumOfRatings = 0;
		for (const s_Position& Trailhead : Trailheads)
			SumOfRatings += RateTrailhead(Map, Trailhead);

		return SumOfRatings;
	}

	public:
		AOC_TEST(Sample, 81)
		AOC_TEST(Input, 1326)
};

/*****************************************************************************/


}
