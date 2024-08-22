#include <cstring>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <string_view>
#include <memory>

using namespace std;



class Solution {
public:
    bool judge_point_24(vector<int> nums) {

        return false;
    }

    bool compute(vector<float> nums) {
        if (nums.size() == 1) {
            if (abs(nums[0] - 24) <= 0.0001) {
                return true;
            } else {
                return false;
            }

        }
        for (int i = 0; i < nums.size() - 1; ++i) {
            vector<float> tmp_res;
            tmp_res.push_back(nums[i] + nums[i + 1]);
            tmp_res.push_back(nums[i] - nums[i + 1]);
            tmp_res.push_back(nums[i] * nums[i + 1]);
            if (nums[i + 1]!= 0) {
                tmp_res.push_back(nums[i] / nums[i + 1]);
            }
            for (int j = 0; j < tmp_res.size(); ++j) {
                vector<float> new_nums(nums);
                new_nums[i] = tmp_res[j];
                new_nums.erase(new_nums.begin() + i + 1);
                if (compute(new_nums)) {
                    return true;
                }
            }
        }
        return false;
    }

    void backtrace(vector<int> nums, vector<int> track, vector<bool>& visited, vector<vector<int>>& res) {
        if (track.size() == nums.size()) {
            res.push_back(track);
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (visited[i]) continue;
            if (find(track.begin(), track.end(), nums[i])!= track.end()) continue;
            visited[i] = true;
            track.push_back(nums[i]);
            backtrace(nums, track, visited, res);
            visited[i] = false;
            track.pop_back();
        }
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> track;
        vector<bool> visited;
        backtrace(nums, track, visited, res);
        return res;
    }

};

int kata09_24_point(int argc, char** argv) {



    return 0;
}
