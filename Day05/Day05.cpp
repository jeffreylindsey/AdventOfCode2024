#include "Precompiled.hpp"

#include "AOCTest.hpp"
#include "LineReader.hpp"
#include "StringUtil.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day05
{


constexpr std::string_view DayString = "Day05";

/*****************************************************************************/

TEST_CLASS(Part1)
{
	struct s_PageOrderingRule
	{
		int Before = 0;
		int After = 0;
	};

	/*=========================================================================
		Returns 0 if the given update is not in the right order.
	-------------------------------------------------------------------------*/
	int GetMiddlePageNumber
	( const std::vector<int>& PagesToProduce
	, const std::vector<int>& CompletePageOrder
	)
	{
		ptrdiff_t LastIndex = -1;

		for (const int Page : PagesToProduce)
		{
			const auto FoundIter = std::ranges::find(CompletePageOrder, Page);
			const ptrdiff_t FoundIndex = FoundIter - CompletePageOrder.begin();

			// The update is not valid if the page is not in order.
			if (FoundIndex <= LastIndex)
				return 0;

			LastIndex = FoundIndex;
		}

		return PagesToProduce[PagesToProduce.size() / 2];
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

		// Build complete page order.
		std::vector<int> CompletePageOrder;
		{
			// Maps pages to the pages that come after it.
			std::unordered_map<int, std::unordered_set<int>> Mapping;

			// Initial mapping.
			for (const s_PageOrderingRule& Rule : PageOrderingRules)
			{
				Mapping[Rule.Before].insert(Rule.After);

				// Make sure the After page exists in the map.
				Mapping[Rule.After];
			}

			// Complete mapping.
			for (auto& [Page, r_Afters] : Mapping)
			{
				std::deque<int> ToCheck(r_Afters.begin(), r_Afters.end());

				for (; !ToCheck.empty(); ToCheck.pop_front())
				{
					const int PageToCheck = ToCheck.front();

					for (const int After : Mapping[PageToCheck])
					{
						if (r_Afters.insert(After).second)
							ToCheck.push_back(After);
					}
				}
			}

			for (const auto& [Page, Afters] : Mapping)
			{
				// Insert the page just before any pages that come after it.
				auto InsertIter = CompletePageOrder.begin();
				for (; InsertIter != CompletePageOrder.end(); ++InsertIter)
				{
					if (Afters.contains(*InsertIter))
						break;
				}

				CompletePageOrder.insert(InsertIter, Page);
			}
		}

		// Verify?
		{
			for (size_t Index = 0; Index < CompletePageOrder.size(); ++Index)
			{
				const int CurrentPage = CompletePageOrder[Index];

				for (const s_PageOrderingRule& Rule : PageOrderingRules)
				{
					if (Rule.Before == CurrentPage)
					{
						const auto AfterPages
							= std::span<int>(CompletePageOrder)
								.subspan(Index + 1);

						if (std::ranges::find(AfterPages, Rule.After)
							== AfterPages.end()
						)
						{
							Assert::Fail();
						}
					}
					else if (Rule.After == CurrentPage)
					{
						const auto BeforePages
							= std::span<int>(CompletePageOrder)
								.subspan(0, Index);

						if (std::ranges::find(BeforePages, Rule.Before)
							== BeforePages.end()
						)
						{
							Assert::Fail();
						}
					}
				}
			}
		}

		int MiddlePageNumberSum = 0;

		for (const std::vector<int>& PagesToProduce : Updates)
		{
			const int MiddlePageNumber
				= GetMiddlePageNumber(PagesToProduce, CompletePageOrder);

			MiddlePageNumberSum += MiddlePageNumber;
		}

		return MiddlePageNumberSum;
	}

	public:
		AOC_TEST(Sample, 143)
		AOC_TEST(Input, 0)
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
