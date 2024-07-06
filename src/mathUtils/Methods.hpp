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

template<class MapType>
inline void removeEntryWithValue(MapType& map, const typename MapType::mapped_type& value)
{
    for (auto itr = map.begin(); itr != map.end();) {
        if (itr->second == value) {
            // Delete current entry and get the next one.
            itr = map.erase(itr);
        } else {
            // Nothing to see here: move onto the next entry.
            ++itr;
        }
    }
}

} // namespace MathUtils::Methods
