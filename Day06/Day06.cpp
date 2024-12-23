#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day06
{


constexpr std::string_view DayString = "Day06";

enum class e_Direction { Up, Down, Left, Right };

/*===========================================================================*/
e_Direction GetRightDirection(const e_Direction Direction)
{
	switch (Direction)
	{
		case e_Direction::Up:
		return e_Direction::Right;

		case e_Direction::Down:
		return e_Direction::Left;

		case e_Direction::Left:
		return e_Direction::Up;

		case e_Direction::Right:
		return e_Direction::Down;
	}

	Assert::Fail();
}

/*===========================================================================*/
e_Direction GetReverseDirection(const e_Direction Direction)
{
	switch (Direction)
	{
		case e_Direction::Up:
		return e_Direction::Down;

		case e_Direction::Down:
		return e_Direction::Up;

		case e_Direction::Left:
		return e_Direction::Right;

		case e_Direction::Right:
		return e_Direction::Left;
	}

	Assert::Fail();
}

/*****************************************************************************/
// s_Position

struct s_Position
{
	//--- Members -------------------------------------------------------------

	int Column = 0;
	int Row = 0;

	//--- Methods -------------------------------------------------------------

	constexpr auto operator<=>(const s_Position&) const = default;

	s_Position InDirection(const e_Direction Direction) const;
};

/*===========================================================================*/
s_Position s_Position::InDirection(const e_Direction Direction) const
{
	s_Position NewPosition = *this;

	switch (Direction)
	{
		case e_Direction::Up:
			--NewPosition.Row;
		break;

		case e_Direction::Down:
			++NewPosition.Row;
		break;

		case e_Direction::Left:
			--NewPosition.Column;
		break;

		case e_Direction::Right:
			++NewPosition.Column;
		break;
	}

	return NewPosition;
}

/*****************************************************************************/
// s_Guard

struct s_Guard
{
	//--- Members -------------------------------------------------------------

	s_Position Position = {};
	e_Direction Direction = e_Direction::Up;

	//--- Methods -------------------------------------------------------------

	s_Position GetForwardPosition() const;

	void TurnRight();
	void StepForward();
};

/*===========================================================================*/
s_Position s_Guard::GetForwardPosition() const
{
	return Position.InDirection(Direction);
}

/*===========================================================================*/
void s_Guard::TurnRight()
{
	Direction = GetRightDirection(Direction);
}

/*===========================================================================*/
void s_Guard::StepForward()
{
	Position = GetForwardPosition();
}

/*****************************************************************************/
// c_Map

class c_Map
{
	//--- Public Methods ------------------------------------------------------
	public:
		void SetWidth(const int Width);
		void SetHeight(const int Height);

		void AddObstruction(const s_Position& Position);

		bool IsObstructionAt(const s_Position& Position) const;
		bool IsInMappedArea(const s_Position& Position) const;

	//--- Private Members -----------------------------------------------------
	private:
		size_t m_Width = 0;
		size_t m_Height = 0;

		std::set<s_Position> m_Obstructions;
};

/*===========================================================================*/
void c_Map::SetWidth(const int Width)
{
	m_Width = Width;
}

/*===========================================================================*/
void c_Map::SetHeight(const int Height)
{
	m_Height = Height;
}

/*===========================================================================*/
void c_Map::AddObstruction(const s_Position& Position)
{
	m_Obstructions.emplace(Position);
}

/*===========================================================================*/
bool c_Map::IsObstructionAt(const s_Position& Position) const
{
	return m_Obstructions.contains(Position);
}

/*===========================================================================*/
bool c_Map::IsInMappedArea(const s_Position& Position) const
{
	return Position.Column >= 0 && Position.Column < m_Width
		&& Position.Row >= 0 && Position.Row < m_Height;
}

/*===========================================================================*/
std::tuple<c_Map, s_Guard> LoadInput(std::ifstream Input)
{
	c_Map Map;
	s_Guard Guard;

	int Row = 0;
	for (const std::string& Line : c_LineReader(Input))
	{
		int Column = 0;
		for (const char Char : Line)
		{
			const s_Position Position{.Column = Column, .Row = Row};

			switch (Char)
			{
				case '#':
					Map.AddObstruction(Position);
				break;

				case '^':
				{
					Guard
						= s_Guard
							{ .Position = Position
							, .Direction = e_Direction::Up
							};
				}
				break;
			}

			++Column;
		}
		Map.SetWidth(Column);

		++Row;
	}
	Map.SetHeight(Row);

	return {Map, Guard};
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		// Load the input.
		const auto [Map, InitialGuard] = LoadInput(std::move(Input));

		std::set<s_Position> PatrolPositions;

		// Simulate the guard's path.
		s_Guard Guard = InitialGuard;
		while (Map.IsInMappedArea(Guard.Position))
		{
			PatrolPositions.insert(Guard.Position);

			const s_Position ForwardPosition = Guard.GetForwardPosition();

			if (Map.IsObstructionAt(ForwardPosition))
				Guard.TurnRight();
			else
				Guard.StepForward();
		}

		return static_cast<int>(PatrolPositions.size());
	}

	public:
		AOC_TEST(Sample, 41)
		AOC_TEST(Input, 4696)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		// Load the input.
		const auto [Map, InitialGuard] = LoadInput(std::move(Input));

		std::set<std::tuple<s_Position, e_Direction>> PotentialObstructions;
		std::set<s_Position> ObstructionPositions;

		// Simulate the guard's path to find potential obstructions.
		s_Guard Guard = InitialGuard;
		while (Map.IsInMappedArea(Guard.Position))
		{
			const s_Position ForwardPosition = Guard.GetForwardPosition();

			// If obstructing the guard at their current position would cause
			// them to reach a previously encountered obstruction, mark this
			// as an obstruction position.
			if (PotentialObstructions.contains
				( std::make_tuple
					( Guard.Position
					, GetRightDirection(Guard.Direction)
					)
				)
			)
			{
				ObstructionPositions.emplace(ForwardPosition);
			}

			if (Map.IsObstructionAt(ForwardPosition))
			{
				// Mark potential obstruction positions that will lead to the
				// guard's current position.
				{
					const e_Direction ReverseDirection
						= GetReverseDirection(Guard.Direction);

					s_Position Position = Guard.Position;

					while (Map.IsInMappedArea(Position)
						&& !Map.IsObstructionAt(Position)
					)
					{
						PotentialObstructions.emplace
							( std::make_tuple(Position, Guard.Direction)
							);

						Position = Position.InDirection(ReverseDirection);
					}
				}

				Guard.TurnRight();
			}
			else
				Guard.StepForward();
		}

		return static_cast<int>(ObstructionPositions.size());
	}

	public:
		AOC_TEST(Sample, 6)
		AOC_TEST(Input, 0)
};

/*****************************************************************************/


}
