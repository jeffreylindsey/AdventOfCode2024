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


}  // namespace n_StringUtil
