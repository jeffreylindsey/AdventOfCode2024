#pragma once

namespace n_StringUtil
{


std::vector<std::string_view> SplitString
	( const std::string_view String
	, const std::string_view Delimiter
	);

std::pair<std::string_view, std::string_view> SplitLeftRight
	( const std::string_view String
	, const std::string_view Delimiter
	);

std::string_view TrimLeadingSpaces(std::string_view String);


}  // namespace n_StringUtil
