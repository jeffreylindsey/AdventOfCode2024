#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day04
{


constexpr std::string_view DayString = "Day04";

/******************************************************************************
// c_WordSearch

This contains the full word search puzzle.
-----------------------------------------------------------------------------*/
class c_WordSearch
{
	//--- Public Methods ------------------------------------------------------
	public:
		void AddLine(const std::string_view Line);

		int CountWord(const std::string_view Word) const;

	//--- Private Types -------------------------------------------------------
	private:
		struct s_Direction
		{
			int dx = 0;  // Column
			int dy = 0;  // Row
		};

	//--- Private Static Members ----------------------------------------------
	private:
		static constexpr s_Direction AllDirections[] =
			{ { 0, -1}  // North
			, { 1, -1}  // NorthEast
			, { 1,  0}  // East
			, { 1,  1}  // SouthEast
			, { 0,  1}  // South
			, {-1,  1}  // SouthWest
			, {-1,  0}  // West
			, {-1, -1}  // NorthWest
			};

	//--- Private Methods -----------------------------------------------------
	private:
		int GetNumColumns() const;
		int GetNumRows() const;

		char GetLetterAt(const int Column, const int Row) const;

		int CountWordInDirection
			( const std::string_view Word
			, const s_Direction& Direction
			) const;

		bool IsWordAt
			( const std::string_view Word
			, int Column
			, int Row
			, const s_Direction& Direction
			) const;

	//--- Private Members -----------------------------------------------------
	private:
		size_t m_LineLength = 0;
		std::vector<char> m_Letters;
};

/*===========================================================================*/
void c_WordSearch::AddLine(const std::string_view Line)
{
	if (m_LineLength == 0)
		m_LineLength = Line.length();

	// Assuming all lines in the puzzle are of equal length.
	Assert::AreEqual(m_LineLength, Line.length());

	m_Letters.insert(m_Letters.end(), Line.begin(), Line.end());
}

/*===========================================================================*/
int c_WordSearch::CountWord(const std::string_view Word) const
{
	int Count = 0;

	for (const s_Direction& Direction : AllDirections)
		Count += CountWordInDirection(Word, Direction);

	return Count;
}

/*===========================================================================*/
int c_WordSearch::GetNumColumns() const
{
	return static_cast<int>(m_LineLength);
}

/*===========================================================================*/
int c_WordSearch::GetNumRows() const
{
	if (m_LineLength == 0)
		return 0;

	return static_cast<int>(m_Letters.size() / m_LineLength);
}

/*=============================================================================
	Return 0 (NUL) if the given Column or Row are outside of the valid area.
-----------------------------------------------------------------------------*/
char c_WordSearch::GetLetterAt(const int Column, const int Row) const
{
	if (Column < 0 || Column > m_LineLength)
		return 0;

	if (Row < 0)
		return 0;

	const size_t Index = Row * m_LineLength + Column;

	if (Index >= m_Letters.size())
		return 0;

	return m_Letters[Index];
}

/*===========================================================================*/
int c_WordSearch::CountWordInDirection
( const std::string_view Word
, const s_Direction& Direction
) const
{
	int Count = 0;

	const int NumColumns = GetNumColumns();
	const int NumRows = GetNumRows();

	for (int Row = 0; Row < NumRows; ++Row)
	{
		for (int Column = 0; Column < NumColumns; ++Column)
		{
			if (IsWordAt(Word, Column, Row, Direction))
				++Count;
		}
	}

	return Count;
}

/*===========================================================================*/
bool c_WordSearch::IsWordAt
( const std::string_view Word
, int Column
, int Row
, const s_Direction& Direction
) const
{
	for (const char Letter : Word)
	{
		if (GetLetterAt(Column, Row) != Letter)
			return false;

		Column += Direction.dx;
		Row += Direction.dy;
	}

	return true;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		c_WordSearch WordSearch;

		// Load the input.
		for (const std::string& Line : c_LineReader(Input))
			WordSearch.AddLine(Line);

		// Do word search.
		const int NumFound = WordSearch.CountWord("XMAS");

		return NumFound;
	}

	public:
		AOC_TEST(Sample, 18)
		AOC_TEST(Input, 2583)
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
		AOC_TEST(Sample, 0)
		AOC_TEST(Input, 0)
};

/*****************************************************************************/


}
