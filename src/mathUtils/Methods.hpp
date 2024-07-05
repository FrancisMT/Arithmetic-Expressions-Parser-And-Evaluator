#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace MathUtils::Methods {

inline std::vector<std::string> splitString(const std::string& stringToSplit, const char splitChar)
{
    std::vector<std::string> output;
    for (const auto& splitValue : stringToSplit | std::ranges::views::split(splitChar)) {
        output.emplace_back(splitValue.begin(), splitValue.end());
    }

    return output;
}

inline constexpr void removeWhiteSpacesFromString(std::string& stringToTrim)
{
    stringToTrim.erase(
          std::remove_if(stringToTrim.begin(),
                         stringToTrim.end(),
                         [](unsigned char stringChar) { return std::isspace(stringChar); }),
          stringToTrim.end());
}

} // namespace MathUtils::Methods
