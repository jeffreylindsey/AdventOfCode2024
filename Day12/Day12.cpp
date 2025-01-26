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
// c_Map2D

template<typename t_Value>
class c_Map2D
{
	//--- Public Static Members -----------------------------------------------
	public:
		static constexpr t_Value DefaultValue{};

	//--- Public Methods ------------------------------------------------------
	public:
		void SetWidth(const int Width);
		void SetHeight(const int Height);

		int Width() const;
		int Height() const;

		const t_Value& GetAt(const int x, const int y) const;
		const t_Value& GetAt(const s_Position& Position) const;

		void SetAt(const int x, const int y, t_Value Value);
		void SetAt(const s_Position& Position, t_Value Value);

	//--- Protected Members ---------------------------------------------------
	protected:
		size_t m_Width = 0;
		std::vector<t_Value> m_Values;
};

/*===========================================================================*/
template<typename t_Value>
void c_Map2D<t_Value>::SetWidth(const int Width)
{
	m_Width = Width;
}

/*===========================================================================*/
template<typename t_Value>
void c_Map2D<t_Value>::SetHeight(const int Height)
{
	m_Values.resize(m_Width * Height);
}

/*===========================================================================*/
template<typename t_Value>
int c_Map2D<t_Value>::Width() const
{
	return static_cast<int>(m_Width);
}

/*===========================================================================*/
template<typename t_Value>
int c_Map2D<t_Value>::Height() const
{
	if (m_Width == 0)
		return 0;

	return static_cast<int>(m_Values.size() / m_Width);
}

/*===========================================================================*/
template<typename t_Value>
const t_Value& c_Map2D<t_Value>::GetAt(const int x, const int y) const
{
	if (x < 0 || x >= m_Width)
		return DefaultValue;

	if (y < 0)
		return DefaultValue;

	const size_t Index = y * m_Width + x;

	if (Index >= m_Values.size())
		return DefaultValue;

	return m_Values[Index];
}

/*===========================================================================*/
template<typename t_Value>
const t_Value& c_Map2D<t_Value>::GetAt(const s_Position& Position) const
{
	return GetAt(Position.x, Position.y);
}

/*===========================================================================*/
template<typename t_Value>
void c_Map2D<t_Value>::SetAt(const int x, const int y, t_Value Value)
{
	if (x < 0 || x >= m_Width)
		return;

	if (y < 0)
		return;

	const size_t Index = y * m_Width + x;

	if (Index >= m_Values.size())
		return;

	m_Values[Index] = std::move(Value);
}

/*===========================================================================*/
template<typename t_Value>
void c_Map2D<t_Value>::SetAt(const s_Position& Position, t_Value Value)
{
	SetAt(Position.x, Position.y, std::move(Value));
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
	: public c_Map2D<s_RegionID>
{
	//--- Public Methods ------------------------------------------------------
	public:
		c_RegionMap(const int Width, const int Height);

		s_RegionID NewRegionID();

		void MergeRegions
			( const s_RegionID FirstRegionID
			, const s_RegionID SecondRegionID
			);

	//--- Private Members -----------------------------------------------------
	private:
		s_RegionID m_NextRegionID{1};
};

/*===========================================================================*/
c_RegionMap::c_RegionMap(const int Width, const int Height)
{
	SetWidth(Width);
	SetHeight(Height);
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
	for (s_RegionID& r_RegionID : m_Values)
	{
		if (r_RegionID == SecondRegionID)
			r_RegionID = FirstRegionID;
	}
}

/*****************************************************************************/

/*===========================================================================*/
c_Map2D<char> Load2DCharMap(std::ifstream& r_Input)
{
	c_Map2D<char> Map;

	for (const std::string& Line : c_LineReader(r_Input))
	{
		const int LineLength = static_cast<int>(Line.length());

		if (Map.Width() == 0)
			Map.SetWidth(LineLength);

		// Assuming all lines in the puzzle are of equal length.
		Assert::AreEqual(Map.Width(), LineLength);

		const int y = Map.Height();

		Map.SetHeight(y + 1);

		int x = 0;
		for (const char Char : Line)
			Map.SetAt(x++, y, Char);
	}

	return Map;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		const c_Map2D<char> PlotMap = Load2DCharMap(Input);

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
						= RegionMap.GetAt(UpPosition);

					const s_RegionID LeftRegionID
						= RegionMap.GetAt(LeftPosition);

					if (UpRegionID != LeftRegionID)
						RegionMap.MergeRegions(UpRegionID, LeftRegionID);

					RegionMap.SetAt(Position, UpRegionID);
				}
				else if (UpPlantType == PlantType)
				{
					RegionMap.SetAt
						( Position
						, RegionMap.GetAt(UpPosition)
						);
				}
				else if (LeftPlantType == PlantType)
				{
					RegionMap.SetAt
						( Position
						, RegionMap.GetAt(LeftPosition)
						);
				}
				else
					RegionMap.SetAt(Position, RegionMap.NewRegionID());
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

				const s_RegionID RegionID = RegionMap.GetAt(Position);

				s_AreaAndPerimeter& r_Region = RegionData[RegionID];

				++r_Region.Area;

				if (RegionMap.GetAt(Position.Up()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetAt(Position.Down()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetAt(Position.Left()) != RegionID)
					++r_Region.Perimeter;
				if (RegionMap.GetAt(Position.Right()) != RegionID)
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
		AOC_TEST(Sample1, 80)
		AOC_TEST(Sample2, 436)
		AOC_TEST(Sample3, 1206)
		AOC_TEST(Sample4, 236)
		AOC_TEST(Sample5, 368)
		AOC_TEST(Input, 0)
};

/*****************************************************************************/


}
