#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day12
{


constexpr std::string_view DayString = "Day12";

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
// c_2DMap

class c_2DMap
{
	//--- Public Methods ------------------------------------------------------
	public:
		void AddLine(const std::string_view Line);

		int Width() const;
		int Height() const;

		char GetAt(const s_Position& Position) const;

	//--- Private Members -----------------------------------------------------
	private:
		size_t m_Width = 0;
		std::vector<char> m_Heights;
};

/*===========================================================================*/
void c_2DMap::AddLine(const std::string_view Line)
{
	if (m_Width == 0)
		m_Width = Line.length();

	// Assuming all lines in the puzzle are of equal length.
	Assert::AreEqual(m_Width, Line.length());

	m_Heights.insert(m_Heights.end(), Line.begin(), Line.end());
}

/*===========================================================================*/
int c_2DMap::Width() const
{
	return static_cast<int>(m_Width);
}

/*===========================================================================*/
int c_2DMap::Height() const
{
	if (m_Width == 0)
		return 0;

	return static_cast<int>(m_Heights.size() / m_Width);
}

/*=============================================================================
	Return 0 (NUL) if the given Column or Row are outside of the valid area.
-----------------------------------------------------------------------------*/
char c_2DMap::GetAt(const s_Position& Position) const
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
// s_RegionID

struct s_RegionID
{
	//--- Members -------------------------------------------------------------

	int Value = 0;

	//--- Methods -------------------------------------------------------------

	constexpr auto operator<=>(const s_RegionID&) const noexcept = default;
};

/*****************************************************************************/
// c_RegionMap

class c_RegionMap
{
	//--- Public Methods ------------------------------------------------------
	public:
		c_RegionMap(const int Width, const int Height);

		s_RegionID GetRegionIDAt(const s_Position& Position) const;

		void SetRegion(const s_Position& Position, const s_RegionID RegionID);

		s_RegionID NewRegionID();

		void MergeRegions
			( const s_RegionID FirstRegionID
			, const s_RegionID SecondRegionID
			);

	//--- Private Members -----------------------------------------------------
	private:
		size_t m_Width = 0;
		std::vector<s_RegionID> m_RegionsByPosition;

		s_RegionID m_NextRegionID{1};
};

/*===========================================================================*/
c_RegionMap::c_RegionMap(const int Width, const int Height)
	: m_Width(Width)
{
	m_RegionsByPosition.resize(Width * Height);
}

/*===========================================================================*/
s_RegionID c_RegionMap::GetRegionIDAt(const s_Position& Position) const
{
	if (Position.x < 0 || Position.x >= m_Width)
		return {};

	if (Position.y < 0)
		return {};

	const size_t Index = Position.y * m_Width + Position.x;

	if (Index >= m_RegionsByPosition.size())
		return {};

	return m_RegionsByPosition[Index];
}

/*===========================================================================*/
void c_RegionMap::SetRegion
( const s_Position& Position
, const s_RegionID RegionID
)
{
	if (Position.x < 0 || Position.x >= m_Width)
		return;

	if (Position.y < 0)
		return;

	const size_t Index = Position.y * m_Width + Position.x;

	if (Index >= m_RegionsByPosition.size())
		return;

	m_RegionsByPosition[Index] = RegionID;
}

/*===========================================================================*/
s_RegionID c_RegionMap::NewRegionID()
{
	s_RegionID Result = m_NextRegionID;

	++m_NextRegionID.Value;

	return Result;
}

/*=============================================================================
	Merges the second region into the first region.
-----------------------------------------------------------------------------*/
void c_RegionMap::MergeRegions
( const s_RegionID FirstRegionID
, const s_RegionID SecondRegionID
)
{
	for (s_RegionID& r_RegionID : m_RegionsByPosition)
	{
		if (r_RegionID == SecondRegionID)
			r_RegionID = FirstRegionID;
	}
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		c_2DMap PlotMap;

		// Load the input.
		for (const std::string& Line : c_LineReader(Input))
			PlotMap.AddLine(Line);

		const int MapWidth = PlotMap.Width();
		const int MapHeight = PlotMap.Height();

		c_RegionMap RegionMap(MapWidth, MapHeight);

		// Identify each of the contiguous regions.
		for (int y = 0; y < MapHeight; ++y)
		{
			for (int x = 0; x < MapWidth; ++x)
			{
				const s_Position Position{x, y};

				const char PlantType = PlotMap.GetAt(Position);

				// Given the order that we iterate through the plots, plots to
				// the left (-x) and up (-y) have already been visited.

				const s_Position UpPosition = Position.Up();
				const s_Position LeftPosition = Position.Left();

				const char UpPlantType = PlotMap.GetAt(UpPosition);
				const char LeftPlantType = PlotMap.GetAt(LeftPosition);

				if (UpPlantType == PlantType && LeftPlantType == PlantType)
				{
					const s_RegionID UpRegionID
						= RegionMap.GetRegionIDAt(UpPosition);

					const s_RegionID LeftRegionID
						= RegionMap.GetRegionIDAt(LeftPosition);

					if (UpRegionID != LeftRegionID)
						RegionMap.MergeRegions(UpRegionID, LeftRegionID);

					RegionMap.SetRegion(Position, UpRegionID);
				}
				else if (UpPlantType == PlantType)
				{
					RegionMap.SetRegion
						( Position
						, RegionMap.GetRegionIDAt(UpPosition)
						);
				}
				else if (LeftPlantType == PlantType)
				{
					RegionMap.SetRegion
						( Position
						, RegionMap.GetRegionIDAt(LeftPosition)
						);
				}
				else
					RegionMap.SetRegion(Position, RegionMap.NewRegionID());
			}
		}

		struct s_AreaAndPerimeter
		{
			int Area = 0;
			int Perimeter = 0;
		};
		std::map<s_RegionID, s_AreaAndPerimeter> RegionData;

		// Calculate the area and perimeter for each region.
		for (int y = 0; y < MapHeight; ++y)
		{
			for (int x = 0; x < MapWidth; ++x)
			{
				const s_Position Position{x, y};

				const s_RegionID RegionID = RegionMap.GetRegionIDAt(Position);

				s_AreaAndPerimeter& r_Region = RegionData[RegionID];

				++r_Region.Area;

				if (RegionMap.GetRegionIDAt(Position.Up()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetRegionIDAt(Position.Down()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetRegionIDAt(Position.Left()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetRegionIDAt(Position.Right()) != RegionID)
					++r_Region.Perimeter;
			}
		}

		// Calculate the total price.
		int TotalPrice = 0;
		for (const auto& [RegionID, AreaAndPerimeter] : RegionData)
			TotalPrice += AreaAndPerimeter.Area * AreaAndPerimeter.Perimeter;

		return TotalPrice;
	}

	public:
		AOC_TEST(Sample1, 140)
		AOC_TEST(Sample2, 772)
		AOC_TEST(Sample3, 1930)
		AOC_TEST(Input, 1424472)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		return 0;
	}

	public:
		AOC_TEST(Sample1, 0)
		AOC_TEST(Sample2, 0)
		AOC_TEST(Sample3, 0)
		AOC_TEST(Input, 0)
};

/*****************************************************************************/


}
