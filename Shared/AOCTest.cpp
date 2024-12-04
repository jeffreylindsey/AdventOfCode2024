#include "Precompiled.hpp"
#include "AOCTest.hpp"

const std::filesystem::path InputsRoot = "../../..";

/*=============================================================================
	Returns an open input file stream for the file at
	InputsRoot/DayString/Name.txt
-----------------------------------------------------------------------------*/
std::ifstream OpenFileFor
( const std::string_view DayString
, const std::string_view Name
)
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	std::filesystem::path InputFilePath = InputsRoot / DayString / Name;
	InputFilePath += ".txt";

	std::ifstream File(InputFilePath);

	if (!File.is_open())
		Assert::Fail(L"Failed to open file.");

	return File;
}

/*===========================================================================*/
