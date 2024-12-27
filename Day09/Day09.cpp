#include "Precompiled.hpp"

#include "AOCTest.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Day09
{


constexpr std::string_view DayString = "Day09";

constexpr int NoFileID = -1;

/*****************************************************************************/

struct s_DiskSpan
{
	//--- Members -------------------------------------------------------------

	int FileID = NoFileID;
	int Length = 0;

	//--- Methods -------------------------------------------------------------

	bool IsEmpty() const;
};

/*===========================================================================*/
bool s_DiskSpan::IsEmpty() const
{
	return (FileID == NoFileID || Length == 0);
}

/*****************************************************************************/

/*===========================================================================*/
std::list<s_DiskSpan> LoadInput(std::ifstream Input)
{
	std::list<s_DiskSpan> Disk;

	int NextFileID = 0;

	while (Input)
	{
		const int FileLengthChar = Input.get();
		const int GapLengthChar = Input.get();

		const int FileLength = FileLengthChar - '0';
		const int GapLength
			= (GapLengthChar == decltype(Input)::traits_type::eof())
				? 0
				: GapLengthChar - '0';

		if (FileLength > 0)
			Disk.push_back(s_DiskSpan{.FileID = NextFileID, .Length = FileLength});

		if (GapLength > 0)
			Disk.push_back(s_DiskSpan{.FileID = NoFileID, .Length = GapLength});

		++NextFileID;
	}

	return Disk;
}

/*===========================================================================*/
int64_t ComputeFilesystemChecksum(const std::list<s_DiskSpan>& Disk)
{
	int64_t Checksum = 0;

	int BlockIndex = 0;

	for (const s_DiskSpan& DiskSpan : Disk)
	{
		for (int SpanIndex = 0; SpanIndex < DiskSpan.Length; ++SpanIndex)
		{
			if (DiskSpan.FileID != NoFileID)
				Checksum += BlockIndex * DiskSpan.FileID;

			++BlockIndex;
		}
	}

	return Checksum;
}

/*===========================================================================*/
void AdvanceToGapSpan
( std::list<s_DiskSpan>::iterator& r_Iter
, const std::list<s_DiskSpan>& Disk
)
{
	while (r_Iter != Disk.end() && r_Iter->FileID != NoFileID)
		++r_Iter;
}

/*===========================================================================*/
void RetreatToFileSpan
( std::list<s_DiskSpan>::iterator& r_Iter
, const std::list<s_DiskSpan>& Disk
)
{
	while (r_Iter != Disk.begin())
	{
		--r_Iter;
		if (r_Iter->FileID != NoFileID)
			break;
	}
}

/*===========================================================================*/
void TrimEndOfDisk(std::list<s_DiskSpan>& r_Disk)
{
	while (!r_Disk.empty() && r_Disk.back().IsEmpty())
		r_Disk.pop_back();
}

/*****************************************************************************/

TEST_CLASS(Part1)
{
	/*=======================================================================*/
	void Compact(std::list<s_DiskSpan>& r_Disk)
	{
		TrimEndOfDisk(r_Disk);

		auto NextGapSpan = r_Disk.begin();

		AdvanceToGapSpan(NextGapSpan, r_Disk);

		while (NextGapSpan != r_Disk.end())
		{
			s_DiskSpan& r_LastFileSpan = r_Disk.back();

			if (r_LastFileSpan.Length < NextGapSpan->Length)
			{
				// Insert before the gap.
				r_Disk.insert(NextGapSpan, r_LastFileSpan);

				// Reduce the size of the gap.
				NextGapSpan->Length -= r_LastFileSpan.Length;

				// Empty the last span.
				r_LastFileSpan.Length = 0;
			}
			else
			{
				// Set the gap to the file.
				NextGapSpan->FileID = r_LastFileSpan.FileID;

				// Reduce the size of the file at the end of the disk.
				r_LastFileSpan.Length -= NextGapSpan->Length;

				AdvanceToGapSpan(NextGapSpan, r_Disk);
			}

			TrimEndOfDisk(r_Disk);
		}
	}

	/*=======================================================================*/
	int64_t Run(std::ifstream Input)
	{
		std::list<s_DiskSpan> Disk = LoadInput(std::move(Input));

		Compact(Disk);

		return ComputeFilesystemChecksum(Disk);
	}

	public:
		AOC_TEST(Sample, 1928ll)
		AOC_TEST(Input, 6359213660505ll)
};

/*****************************************************************************/

TEST_CLASS(Part2)
{
	/*=======================================================================*/
	int64_t Run(std::ifstream Input)
	{
		return 0;
	}

	public:
		AOC_TEST(Sample, 0ll)
		AOC_TEST(Input, 0ll)
};

/*****************************************************************************/


}
