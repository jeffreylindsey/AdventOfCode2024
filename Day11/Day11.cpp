#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day11
{


constexpr std::string_view DayString = "Day11";

// Maps a number to the number of stones that have that number engraved on it.
using t_PebbleSummary = std::map<uint64_t, size_t>;

/*===========================================================================*/
t_PebbleSummary ReadInput(std::ifstream& r_Input)
{
	t_PebbleSummary PebbleSummary;

	// Read the entire input into a string.
	const std::string InputString(std::istreambuf_iterator<char>(r_Input), {});

	// Parse the input.
	for (const std::string_view NumberAsString
		: n_StringUtil::SplitString(InputString, " ")
	)
	{
		++PebbleSummary[std::stoi(std::string(NumberAsString))];
	}

	return PebbleSummary;
}

/*===========================================================================*/
bool TrySplitDigits
( const uint64_t OriginalNumber
, uint64_t& r_LeftNumber
, uint64_t& r_RightNumber
)
{
	/*

	10^0 = 1
	10^1 = 10
	10^2 = 100
	10^3 = 1000

	log10(1000) = 3
	log10(100) = 2
	log10(10) = 1
	log10(9) = 0.9ish
	log10(1) = 0

	1234, digits: 4, half: 2

	10^2 = 100

	1234 / 100 = 12
	1234 % 100 = 34

	*/
	const int NumDigits
		= static_cast<int>(std::floor(std::log10(OriginalNumber))) + 1;

	if (NumDigits % 2 != 0)
		return false;

	const int HalfDigits = NumDigits / 2;

	const uint64_t SplitMultiple
		= static_cast<uint64_t>(std::pow(10, HalfDigits));

	r_LeftNumber = OriginalNumber / SplitMultiple;
	r_RightNumber = OriginalNumber % SplitMultiple;

	return true;
}

/*===========================================================================*/
t_PebbleSummary GetBlinkResult(t_PebbleSummary PriorSummary)
{
	t_PebbleSummary NewSummary;

	for (const auto& [OriginalNumber, Count] : PriorSummary)
	{
		uint64_t LeftNumber, RightNumber;

		if (OriginalNumber == 0)
			NewSummary[1] += Count;
		else if (TrySplitDigits(OriginalNumber, LeftNumber, RightNumber))
		{
			NewSummary[LeftNumber] += Count;
			NewSummary[RightNumber] += Count;
		}
		else
			NewSummary[OriginalNumber * 2024] += Count;
	}

	return NewSummary;
}

/*===========================================================================*/
size_t SimulateBlinks(t_PebbleSummary PebbleSummary, const int NumberOfBlinks)
{
	// Simulate the blinks.
	for (int BlinksRemaining = NumberOfBlinks;
		BlinksRemaining > 0;
		--BlinksRemaining
	)
	{
		PebbleSummary = GetBlinkResult(std::move(PebbleSummary));
	}

	// Count the stones.
	size_t TotalCount = 0;
	for (const auto& [Number, Count] : PebbleSummary)
		TotalCount += Count;

	return TotalCount;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	size_t Run(std::ifstream Input)
	{
		return SimulateBlinks(ReadInput(Input), 25);
	}

	public:
		AOC_TEST(Sample, 55312ull)
		AOC_TEST(Input, 172484ull)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	size_t Run(std::ifstream Input)
	{
		return SimulateBlinks(ReadInput(Input), 75);
	}

	public:
		//AOC_TEST(Sample, 0ull)
		AOC_TEST(Input, 205913561055242ull)
};

/*****************************************************************************/


}
