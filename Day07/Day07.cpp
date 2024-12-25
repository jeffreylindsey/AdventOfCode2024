#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day07
{


constexpr std::string_view DayString = "Day07";

enum class e_Operator { Add, Multiply, Concatenate };

/*===========================================================================*/
auto ParseLine(const std::string_view Line)
{
	const auto [TestValueString, NumbersString]
		= n_StringUtil::SplitLeftRight(Line, ":");

	const int64_t TestValue = std::stoll(std::string(TestValueString));

	std::vector<int> Numbers;
	for (const std::string_view NumberAsString
		: n_StringUtil::SplitString
			( n_StringUtil::TrimLeadingSpaces(NumbersString)
			, " "
			)
	)
	{
		Numbers.push_back(std::stoi(std::string(NumberAsString)));
	}

	return std::make_pair(TestValue, Numbers);
}

/*===========================================================================*/
constexpr size_t IntPow(const size_t Base, size_t Exponent)
{
	size_t Result = 1;

	for (; Exponent > 0; --Exponent)
		Result *= Base;

	return Result;
}

/*===========================================================================*/
e_Operator GetOperatorForIndex
( const size_t CombinationIndex
, const size_t OperatorIndex
, const std::span<const e_Operator> Operators
)
{
	const size_t NumOperators = Operators.size();

	/*

	xx| 0 | 1 | 2
	--|---|---|---
	0 | 0 |
	1 | 1 |
	2 | 0 | 1
	3 | 1 | 1
	4 | 0 | 0 | 1
	5 | 1 | 0 | 1
	6 | 0 | 1 | 1
	7 | 1 | 1 | 1

	*/

	const size_t OperatorValue
		= (CombinationIndex / IntPow(NumOperators, OperatorIndex))
			% NumOperators;

	return Operators[OperatorValue];
}

/*=============================================================================
	Returns true only if a result matching the given TestValue is found.
-----------------------------------------------------------------------------*/
bool SolveTest
( const int64_t TestValue
, const std::vector<int>& Numbers
, const std::span<const e_Operator> Operators
)
{
	const size_t NumOperators = Operators.size();

	if (Numbers.size() < 1)
		return false;

	if (Numbers.size() == 1)
		return (Numbers.front() == TestValue);

	const size_t NumCombinations = IntPow(NumOperators, Numbers.size() - 1);

	for (size_t CombinationIndex = 0;
		CombinationIndex < NumCombinations;
		++CombinationIndex
	)
	{
		int64_t Result = Numbers.front();

		size_t OperatorIndex = 0;
		for (const int Number : Numbers | std::views::drop(1))
		{
			const e_Operator Operator
				= GetOperatorForIndex
					( CombinationIndex
					, OperatorIndex
					, Operators
					);

			switch (Operator)
			{
				case e_Operator::Add:
					Result += Number;
				break;

				case e_Operator::Multiply:
					Result *= Number;
				break;

				case e_Operator::Concatenate:
				{
					Result
						= std::stoll
							( std::to_string(Result)
								+ std::to_string(Number)
							);
				}
				break;
			}

			// Early exit if the result has exceeded the test value,
			// because the result is only going to get larger.
			if (Result > TestValue)
				break;

			++OperatorIndex;
		}

		if (Result == TestValue)
			return true;
	}

	return false;
}

/*===========================================================================*/
int64_t RunUsingOperators
( std::ifstream Input
, const std::span<const e_Operator> Operators
)
{
	int64_t TotalCalibrationResult = 0;

	for (const std::string& Line : c_LineReader(Input))
	{
		const auto [TestValue, Numbers] = ParseLine(Line);

		if (SolveTest(TestValue, Numbers, Operators))
			TotalCalibrationResult += TestValue;
	}

	return TotalCalibrationResult;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	static constexpr std::array<e_Operator, 2> Part1Operators
		= { e_Operator::Add, e_Operator::Multiply };

	/*=======================================================================*/
	int64_t Run(std::ifstream Input)
	{
		return RunUsingOperators(std::move(Input), Part1Operators);
	}

	public:
		AOC_TEST(Sample, 3749ll)
		AOC_TEST(Input, 2437272016585ll)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	static constexpr std::array<e_Operator, 3> Part2Operators
		= { e_Operator::Add, e_Operator::Multiply, e_Operator::Concatenate };

	/*=======================================================================*/
	int64_t Run(std::ifstream Input)
	{
		return RunUsingOperators(std::move(Input), Part2Operators);
	}

	public:
		AOC_TEST(Sample, 11387ll)
		AOC_TEST(Input, 162987117690649ll)
};

/*****************************************************************************/


}
