#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day01
{


constexpr std::string_view DayString = "Day01";

using t_LocationID = int;

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		// Note: Using std::greater to sort the queue so that the smallest
		// numbers are first.
		using t_LocationIDList
			= std::priority_queue
				< t_LocationID
				, std::vector<t_LocationID>
				, std::greater<t_LocationID>
				>;

		t_LocationIDList LeftList;
		t_LocationIDList RightList;

		for (const std::string& Line : c_LineReader(Input))
		{
			t_LocationID LeftLocationID = 0;
			t_LocationID RightLocationID = 0;

			std::stringstream Stream(Line);
			Stream >> LeftLocationID >> RightLocationID;

			LeftList.push(LeftLocationID);
			RightList.push(RightLocationID);
		}

		// Assuming both lists have the same number of elements.
		Assert::AreEqual(LeftList.size(), RightList.size());

		int TotalDistance = 0;

		// Accumulate the total distance from both lists.
		for (;
			!LeftList.empty() && !RightList.empty();
			LeftList.pop(), RightList.pop()
		)
		{
			const int Distance = std::abs(LeftList.top() - RightList.top());
			TotalDistance += Distance;
		}

		return TotalDistance;
	}

	public:
		AOC_TEST(Sample, 11)
		AOC_TEST(Input, 765748)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		std::vector<t_LocationID> LeftList;
		std::unordered_map<t_LocationID, int> RightListCounts;

		for (const std::string& Line : c_LineReader(Input))
		{
			t_LocationID LeftLocationID = 0;
			t_LocationID RightLocationID = 0;

			std::stringstream Stream(Line);
			Stream >> LeftLocationID >> RightLocationID;

			LeftList.push_back(LeftLocationID);
			++RightListCounts[RightLocationID];
		}

		int SimilarityScore = 0;

		for (const t_LocationID LocationID : LeftList)
		{
			const auto FoundIter = RightListCounts.find(LocationID);
			if (FoundIter == RightListCounts.end())
				continue;

			const int Count = FoundIter->second;

			SimilarityScore += LocationID * Count;
		}

		return SimilarityScore;
	}

	public:
		AOC_TEST(Sample, 31)
		AOC_TEST(Input, 27732508)
};

/*****************************************************************************/


}
