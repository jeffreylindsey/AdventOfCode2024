#pragma once

/*=============================================================================
	The following are expected to be defined in the scope that uses this macro:
	* Run: a function that takes a std::iftream and returns a value.
	* DayString: a std::string_view such as "Day01".

	Name is typically expected to be either Sample or Input, which will
	correspond with the Sample.txt and Input.txt input files.

	The Expected value will be compared to the return value of Run.
-----------------------------------------------------------------------------*/
#define AOC_TEST(Name, Expected) TEST_METHOD(Name) \
	{ Assert::AreEqual(Expected, Run(::OpenFileFor(DayString, #Name))); }

std::ifstream OpenFileFor
	( const std::string_view DayString
	, const std::string_view Name
	);
