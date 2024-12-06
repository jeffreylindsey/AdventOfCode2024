#include "Precompiled.hpp"

#include "AOCTest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day03
{


constexpr std::string_view DayString = "Day03";

struct s_MultiplyOperation
{
	int Left = 0;
	int Right = 0;
};

/*=============================================================================
	If r_Remaining begins with the given Match, returns true and modifies
	r_Remaining to remove the given Match prefix.
-----------------------------------------------------------------------------*/
bool ParseMatch(std::string_view& r_Remaining, const std::string_view Match)
{
	if (!r_Remaining.starts_with(Match))
		return false;

	r_Remaining.remove_prefix(Match.length());
	return true;
}

/*===========================================================================*/
std::optional<int> ParseNumber(std::string_view& r_Remaining)
{
	int NumDigits = 0;
	for (const char Char : r_Remaining)
	{
		if (std::isdigit(Char))
			++NumDigits;
		else
			break;
	}

	if (NumDigits == 0)
		return std::nullopt;

	if (NumDigits > 3)
		return std::nullopt;

	const std::string_view Digits = r_Remaining.substr(0, NumDigits);

	const int Number = std::stoi(std::string(Digits));

	r_Remaining.remove_prefix(NumDigits);

	return Number;
}

/*===========================================================================*/
std::optional<s_MultiplyOperation> Parse(std::string_view& r_Remaining)
{
	// Do not modify r_Remaining until there is a successful parse.
	std::string_view Local = r_Remaining;

	if (!ParseMatch(Local, "mul("))
		return std::nullopt;

	const std::optional<int> FirstNumber = ParseNumber(Local);
	if (!FirstNumber.has_value())
		return std::nullopt;

	if (!ParseMatch(Local, ","))
		return std::nullopt;

	const std::optional<int> SecondNumber = ParseNumber(Local);
	if (!SecondNumber.has_value())
		return std::nullopt;

	if (!ParseMatch(Local, ")"))
		return std::nullopt;

	r_Remaining = Local;

	return s_MultiplyOperation{.Left = *FirstNumber, .Right = *SecondNumber};
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		// Read the entire input into a string.
		const std::string InputString(std::istreambuf_iterator<char>(Input), {});

		std::vector<s_MultiplyOperation> MultiplyOperations;

		// Parse the string for all of the multiply operations.
		std::string_view Remaining = InputString;
		while (!Remaining.empty())
		{
			const std::optional<s_MultiplyOperation> MultiplyOperation
				= Parse(Remaining);

			if (MultiplyOperation.has_value())
				MultiplyOperations.push_back(*MultiplyOperation);
			else
				Remaining.remove_prefix(1);  // Advance by a single character.
		}

		// Sum the results of the multiply operations.
		int Result = 0;
		for (const s_MultiplyOperation& MultiplyOperation : MultiplyOperations)
			Result += MultiplyOperation.Left * MultiplyOperation.Right;

		return Result;
	}

	public:
		AOC_TEST(Sample, 161)
		AOC_TEST(Input, 182619815)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		// Read the entire input into a string.
		const std::string InputString(std::istreambuf_iterator<char>(Input), {});

		std::vector<s_MultiplyOperation> MultiplyOperations;

		bool IsEnabled = true;

		// Parse the string for all of the multiply operations.
		std::string_view Remaining = InputString;
		while (!Remaining.empty())
		{
			if (ParseMatch(Remaining, "do()"))
			{
				IsEnabled = true;
				continue;
			}

			if (ParseMatch(Remaining, "don't()"))
			{
				IsEnabled = false;
				continue;
			}

			const std::optional<s_MultiplyOperation> MultiplyOperation
				= Parse(Remaining);

			if (MultiplyOperation.has_value())
			{
				if (IsEnabled)
					MultiplyOperations.push_back(*MultiplyOperation);
			}
			else
				Remaining.remove_prefix(1);  // Advance by a single character.
		}

		// Sum the results of the multiply operations.
		int Result = 0;
		for (const s_MultiplyOperation& MultiplyOperation : MultiplyOperations)
			Result += MultiplyOperation.Left * MultiplyOperation.Right;

		return Result;
	}

	public:
		AOC_TEST(Sample2, 48)
		AOC_TEST(Input, 80747545)
};

/*****************************************************************************/


}
