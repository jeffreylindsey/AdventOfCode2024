#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day05
{


constexpr std::string_view DayString = "Day05";

/*=======================================================================*/
template<typename t_Container, typename t_Value>
bool Contains(t_Container Container, t_Value Value)
{
	return (std::ranges::find(Container, Value) != Container.end());
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	struct s_PageOrderingRule
	{
		int Before = 0;
		int After = 0;
	};

	/*=======================================================================*/
	bool ArePagesCorrectlyOrdered
	( const std::span<const int> PagesToProduce
	, const std::vector<s_PageOrderingRule>& PageOrderingRules
	)
	{
		// Iterate through each page.
		for (size_t PageIndexToCheck = 0;
			PageIndexToCheck < PagesToProduce.size();
			++PageIndexToCheck
		)
		{
			const int PageToCheck = PagesToProduce[PageIndexToCheck];

			for (const s_PageOrderingRule& Rule : PageOrderingRules)
			{
				// Check if this rule applies to the current page.
				if (PageToCheck == Rule.Before)
				{
					// This rule indicates that this page must be before
					// another page.  If that other page is found before this
					// page, then the order is not correct.

					const auto PagesBefore
						= PagesToProduce.subspan(0, PageIndexToCheck);

					if (Contains(PagesBefore, Rule.After))
						return false;
				}
				else if (PageToCheck == Rule.After)
				{
					// This rule indicates that this page must be after
					// another page.  If that other page is found is found
					// after this page, then the order is not correct.

					const auto PagesAfter
						= PagesToProduce.subspan(PageIndexToCheck + 1);

					if (Contains(PagesAfter, Rule.Before))
						return false;
				}
			}
		}

		return true;
	}

	/*=======================================================================*/
	int Run(std::ifstream Input)
	{
		std::vector<s_PageOrderingRule> PageOrderingRules;
		std::vector<std::vector<int>> Updates;

		enum class e_ReadState { PageOrderingRules, PagesToProduce } ReadState
			= e_ReadState::PageOrderingRules;

		// Load the input.
		for (const std::string& Line : c_LineReader(Input))
		{
			if (ReadState == e_ReadState::PageOrderingRules)
			{
				if (Line.empty())
				{
					ReadState = e_ReadState::PagesToProduce;
					continue;
				}

				const auto Split = n_StringUtil::SplitString(Line, "|");

				Assert::AreEqual(2ull, Split.size());

				PageOrderingRules.emplace_back
					( s_PageOrderingRule
						{ .Before = std::stoi(std::string(Split[0]))
						, .After = std::stoi(std::string(Split[1]))
						}
					);
			}
			else if (ReadState == e_ReadState::PagesToProduce)
			{
				std::vector<int> PagesToProduce;

				for (const std::string_view PageNumber
					: n_StringUtil::SplitString(Line, ",")
				)
				{
					PagesToProduce.emplace_back
						( std::stoi(std::string(PageNumber))
						);
				}

				Updates.emplace_back(std::move(PagesToProduce));
			}
		}

		int MiddlePageNumberSum = 0;

		for (const std::vector<int>& PagesToProduce : Updates)
		{
			if (ArePagesCorrectlyOrdered(PagesToProduce, PageOrderingRules))
			{
				const size_t MiddlePageIndex = PagesToProduce.size() / 2;

				MiddlePageNumberSum += PagesToProduce[MiddlePageIndex];
			}
		}

		return MiddlePageNumberSum;
	}

	public:
		AOC_TEST(Sample, 143)
		AOC_TEST_IGNORE(Input, 0)
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
		AOC_TEST_IGNORE(Sample, 0)
		AOC_TEST_IGNORE(Input, 0)
};

/*****************************************************************************/


}
