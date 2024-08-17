#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;


class Solution {
public:
    void find_min_max(vector<int>& nums, int left, int right, int& min, int& max) {
        if (left == right) {
            min = max = nums[left];
            return;
        } else if(left + 1 == right) {
            if (nums[left] < nums[right]) {
                min = nums[left];
                max = nums[right];
            } else {
                min = nums[right];
                max = nums[left];
            }
            return;
        } else {
            int mid = (left + right) / 2;
            int min_left, max_left;
            int min_right, max_right;
            find_min_max(nums, left, mid, min_left, max_left);
            find_min_max(nums, mid + 1, right, min_right, max_right);
            min = min_left < min_right? min_left : min_right;
            max = max_left > max_right? max_left : max_right;
        }
    }
};

int kata103_min_max(int argc, char** argv) {
    vector<int> nums = {13,2,7,11,27,15,18};
    Solution s;
    int min = 0;
    int max = INT_MAX;
    s.find_min_max(nums, 0, nums.size() - 1, min, max);
    cout << "min = " << min << " max = " << max << endl;
    return 0;
}
