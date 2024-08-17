#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

/*

发现一个发现拼写错误的捷径：

1. 三个同样的字母连在一起，一定是拼写错误，去掉一个的就好啦：比如 helllo -> hello
2. 两对一样的字母（AABB型）连在一起，一定是拼写错误，去掉第二对的一个字母就好啦：比如 helloo -> hello
3. 上面的规则优先“从左到右”匹配，即如果是AABBCC，虽然AABB和BBCC都是错误拼写，应该优先考虑修复AABB，结果为AABCC


输入描述：
第一行包括一个数字N，表示本次用例包括多少个待校验的字符串。

后面跟随N行，每行为一个待校验的字符串。
输出描述：
N行，每行包括一个被修复后的字符串。
示例1
    输入例子：
    2
    helloo
    wooooooow
    输出例子：
    hello
    woow
示例2
    输入例子：
    1
    nowcoder
    输出例子：
    nowcoder
*/
enum class SpellPattern {
    kNone,
    kAaa,
    kAabb
};

class Solution {
public:

    string fix_spell_error(const string& line) {
        SpellPattern spellPattern = SpellPattern::kNone;

        int same_char_count = 0;
        int second_same_char_count = 0;

        char last_char = '\0';
        string fixed_string = "";

        for (size_t i = 0; i < line.length(); i++) {
            if (i == 0) {
                last_char = line[i];
                fixed_string += line[i];
                same_char_count = 1;
                continue;
            }
            if (last_char == line[i]) {
                same_char_count++;
                if (same_char_count >= 3) {
                    spellPattern = SpellPattern::kAaa;
                } else if (second_same_char_count >= 1) {
                    spellPattern = SpellPattern::kAabb;
                    second_same_char_count ++;
                }
            } else {
                if (same_char_count >= 2) {
                    second_same_char_count = 1;
                }
                same_char_count = 1;
            }
            last_char = line[i];
            if (spellPattern == SpellPattern::kNone) {
                fixed_string += line[i];
            } else {
                //not append this char
                spellPattern = SpellPattern::kNone;
            }
        }

        return fixed_string;
    }

    void fix_spell_errors(vector<string>& lines) {
        for (auto& line: lines) {
            if (line.empty()) {
                continue;
            }
            auto word = fix_spell_error(line);
            if (word != line) {
                cout << line << " -> " << word << endl;
                line = word;
            }
        }
    }
};

int kata105_fix_spell_error(int argc, char** argv) {

    std::vector<std::string> lines = {"", "helloo", "wooooooow", "runnner", "nowcoder", "aabbcc"};

    Solution s;
    s.fix_spell_errors(lines);
    cout << "fixed words: " << endl;
    for(auto& str: lines) {
        cout << str << endl;
    }
    return 0;
}
