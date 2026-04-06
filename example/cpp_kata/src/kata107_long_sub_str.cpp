#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "kata_helper.h"
using namespace std;

//refer to https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

/*
Given a string s, find the length of the longest
substring without repeating characters.


Example 1:

    Input: s = "abcabcbb"
    Output: 3
    Explanation: The answer is "abc", with the length of 3.

Example 2:

    Input: s = "bbbbb"
    Output: 1
    Explanation: The answer is "b", with the length of 1.

Example 3:

    Input: s = "pwwkew"
    Output: 3
    Explanation: The answer is "wke", with the length of 3.
    Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.


Constraints:

    0 <= s.length <= 5 * 104
    s consists of English letters, digits, symbols and spaces.

*/
class Solution {
public:

    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> m;
        int maxlen = 0;
        for(int i = 0, j = 0; j < s.size(); j++){
            m[s[j]]++;
            while(m[s[j]] > 1) m[s[i++]]--;
            maxlen = max(maxlen, j - i + 1);
        }
        return maxlen;
    }

    int find_longest_substr(const std::string& str) {
        return 0;
    }
};

int kata107_long_sub_str(int argc, char** argv) {


    return 0;
}
