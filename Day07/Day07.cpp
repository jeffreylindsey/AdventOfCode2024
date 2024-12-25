#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day07
{


constexpr std::string_view DayString = "Day07";

/*===========================================================================*/
constexpr size_t IntPow(const size_t Base, size_t Exponent)
{
	size_t Result = 1;

	for (; Exponent > 0; --Exponent)
		Result *= Base;

	return Result;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	enum class e_Operator { Add, Multiply };

	static constexpr std::array<e_Operator, 2> AllOperators
		= { e_Operator::Add, e_Operator::Multiply };

	static constexpr size_t NumOperators = AllOperators.size();

	/*=======================================================================*/
	e_Operator GetOperatorForIndex
	( const size_t CombinationIndex
	, const size_t OperatorIndex
	)
	{
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

		return AllOperators[OperatorValue];
	}

	/*=========================================================================
		Returns true only if a result matching the given TestValue is found.
	-------------------------------------------------------------------------*/
	bool SolveTest(const int64_t TestValue, const std::vector<int>& Numbers)
	{
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
				switch (GetOperatorForIndex(CombinationIndex, OperatorIndex))
				{
					case e_Operator::Add:
						Result += Number;
					break;

					case e_Operator::Multiply:
						Result *= Number;
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

	/*=======================================================================*/
	int64_t Run(std::ifstream Input)
	{
		int64_t TotalCalibrationResult = 0;

		for (const std::string& Line : c_LineReader(Input))
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

			if (SolveTest(TestValue, Numbers))
				TotalCalibrationResult += TestValue;
		}

		return TotalCalibrationResult;
	}

	public:
		AOC_TEST(Sample, 3749ll)
		AOC_TEST(Input, 2437272016585ll)
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
