#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day02
{


constexpr std::string_view DayString = "Day02";

/*===========================================================================*/
bool IsReportSafe(const std::vector<int>& Report)
{
	if (Report.size() < 2)
		return true;

	const bool IsIncreasing = (Report[0] < Report[1]);

	int PriorLevel = Report[0];
	for (size_t Index = 1; Index < Report.size(); ++Index)
	{
		const int Level = Report[Index];
		const int Difference = (Level - PriorLevel);

		if (Difference == 0)
			return false;

		if (IsIncreasing && Difference < 0)
			return false;
		if (!IsIncreasing && Difference > 0)
			return false;

		if (std::abs(Difference) > 3)
			return false;

		PriorLevel = Level;
	}

	return true;
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		int NumReportsSafe = 0;

		for (const std::string& Line : c_LineReader(Input))
		{
			std::istringstream LineStream(Line);

			std::vector<int> Report;

			std::string Level;
			while (std::getline(LineStream, Level, ' '))
				Report.push_back(std::stoi(Level));

			if (IsReportSafe(Report))
				++NumReportsSafe;
		}

		return NumReportsSafe;
	}

	public:
		AOC_TEST(Sample, 2)
		AOC_TEST(Input, 279)
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
