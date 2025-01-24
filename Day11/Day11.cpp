#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day11
{


constexpr std::string_view DayString = "Day11";

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
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

	/*=======================================================================*/
	std::vector<uint64_t> GetBlinkResult(std::vector<uint64_t> PriorNumbers)
	{
		std::vector<uint64_t> NewNumbers;
		NewNumbers.reserve(PriorNumbers.size() * 2);

		for (const uint64_t OriginalNumber : PriorNumbers)
		{
			uint64_t LeftNumber, RightNumber;

			if (OriginalNumber == 0)
				NewNumbers.push_back(1);
			else if (TrySplitDigits(OriginalNumber, LeftNumber, RightNumber))
			{
				NewNumbers.push_back(LeftNumber);
				NewNumbers.push_back(RightNumber);
			}
			else
				NewNumbers.push_back(OriginalNumber * 2024);
		}

		return NewNumbers;
	}

	/*=======================================================================*/
	size_t Run(std::ifstream Input)
	{
		constexpr int NumberOfBlinks = 25;

		// Read the entire input into a string.
		const std::string InputString(std::istreambuf_iterator<char>(Input), {});

		// Parse the input.
		std::vector<uint64_t> Numbers;
		for (const std::string_view NumberAsString
			: n_StringUtil::SplitString(InputString, " ")
		)
		{
			Numbers.push_back(std::stoi(std::string(NumberAsString)));
		}

		// Simulate the blinks.
		for (int BlinksRemaining = NumberOfBlinks;
			BlinksRemaining > 0;
			--BlinksRemaining
		)
		{
			Numbers = GetBlinkResult(std::move(Numbers));
		}

		return Numbers.size();
	}

	public:
		AOC_TEST(Sample, 55312ull)
		AOC_TEST(Input, 172484ull)
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
