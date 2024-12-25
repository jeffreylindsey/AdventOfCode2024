#include "Precompiled.hpp"
#include "StringUtil.hpp"

namespace n_StringUtil
{


/*===========================================================================*/
std::vector<std::string_view> SplitString
( const std::string_view String
, const std::string_view Delimiter
)
{
	std::vector<std::string_view> Result;

	std::string_view Remaining = String;

	while (true)
	{
		const size_t DelimiterOffset = Remaining.find(Delimiter);

		if (DelimiterOffset == Remaining.npos)
		{
			Result.push_back(Remaining);
			break;
		}

		Result.emplace_back(Remaining.substr(0, DelimiterOffset));

		Remaining.remove_prefix(DelimiterOffset + Delimiter.length());
	}

	return Result;
}

/*===========================================================================*/
std::pair<std::string_view, std::string_view> SplitLeftRight
( const std::string_view String
, const std::string_view Delimiter
)
{
	const size_t DelimiterOffset = String.find(Delimiter);

	if (DelimiterOffset == String.npos)
		return {String, {}};

	const std::string_view Left = String.substr(0, DelimiterOffset);

	const std::string_view Right
		= String.substr(DelimiterOffset + Delimiter.length());

	return {Left, Right};
}

/*===========================================================================*/
std::string_view TrimLeadingSpaces(std::string_view String)
{
	while (String.starts_with(' '))
		String.remove_prefix(1);

	return String;
}

/*===========================================================================*/


}  // namespace n_StringUtil
