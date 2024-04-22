#ifndef __RAINDROP_UTILS_LEVENSHTEIN_HPP__
#define __RAINDROP_UTILS_LEVENSHTEIN_HPP__

#include "common.hpp"

namespace Raindrop::Utils{
	std::size_t calculateLevenshteinDistance(const std::string& str1, const std::string& str2) {
		const std::size_t len1 = str1.size();
		const std::size_t len2 = str2.size();
		std::vector<std::vector<std::size_t>> dp(len1 + 1, std::vector<std::size_t>(len2 + 1, 0));

		for (std::size_t i = 0; i <= len1; ++i) {
			for (std::size_t j = 0; j <= len2; ++j) {
				if (i == 0)
					dp[i][j] = j;
				else if (j == 0)
					dp[i][j] = i;
				else if (str1[i - 1] == str2[j - 1])
					dp[i][j] = dp[i - 1][j - 1];
				else
					dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
			}
		}
		return dp[len1][len2];
	}
}

#endif