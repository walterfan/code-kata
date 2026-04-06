#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>
#include "kata_helper.h"

using namespace std;


class Solution {
public:
    double findMedianSortedArrays(vector<int>& arr1, vector<int>& arr2) {
        int n1 = arr1.size();
        int n2 = arr2.size();

        // Ensure arr1 is the smaller array
        if (n1 > n2) {
            return findMedianSortedArrays(arr2, arr1);
        }

        int low = 0, high = n1;

        while (low <= high) {
            int partition1 = (low + high) / 2;
            int partition2 = (n1 + n2 + 1) / 2 - partition1;

            int maxLeft1 = (partition1 == 0) ? INT_MIN : arr1[partition1 - 1];
            int minRight1 = (partition1 == n1) ? INT_MAX : arr1[partition1];

            int maxLeft2 = (partition2 == 0) ? INT_MIN : arr2[partition2 - 1];
            int minRight2 = (partition2 == n2) ? INT_MAX : arr2[partition2];

            if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
                if ((n1 + n2) % 2 == 0) {
                    return (double)(max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2;
                } else {
                    return (double)max(maxLeft1, maxLeft2);
                }
            } else if (maxLeft1 > minRight2) {
                high = partition1 - 1;
            } else {
                low = partition1 + 1;
            }
        }

        // If input arrays are not sorted or something goes wrong
        throw invalid_argument("Input arrays are not sorted");
    }

    //将数组以 arr[low] 分成两部分，使得左边的数都小于等于右边的数
    void partition(vector<int>& arr, int low, int high, int* pos) {
        int key = arr[low];
        while(low < high) {
            //右移 high 索引，直到找到比 key 小的数
            while(low < high && arr[high] > key) {
                high--;
            }
            //将 high 索引所指的数放到 low 索引所指的位置
            arr[low] = arr[high];
            //左移 low 索引，直到找到比 key 大的数
            while(low < high && arr[low] < key) {
                low++;
            }
            //将 low 索引所指的数放到 high 索引所指的位置
            arr[high] = arr[low];
        }
        arr[low] = key;
        *pos = low;
    }

    double find_median(vector<int>& nums) {
        int low = 0;
        int high = nums.size() - 1;
        int mid = (low + high)/2;
        int pos = 0;
        while(true) {
            partition(nums, low, high, &pos);
            if (pos == mid) {
                break;
            } else if (pos > mid) {
                high = pos - 1;
            } else {
                low = pos + 1;
            }
        }

        // 如果数组的长度为偶数，则中位数为 (nums[mid] + nums[mid + 1]) / 2
        if (nums.size() % 2 == 0) {
            return (double)(nums[mid] + nums[mid + 1]) / 2;
        } else {
            // 如果数组的长度为奇数，则中位数为 nums[mid]
            return (double)nums[mid];
        }
    }
};

int kata104_find_median(int argc, char** argv) {
    vector<int> nums1 = {2,7,11,13,15,18};
    vector<int> nums2 = {3,5,8,11,14,16,27};
    print_array(nums1);
    print_array(nums2);

    Solution s;
    double median = s.findMedianSortedArrays(nums1, nums2);
    cout << "two sorted vectors' median = " << median << "\n\n";

    //1, 3, 5, 7, 9,11
    vector<int> nums = {7,5,3,1,11,9};
    print_array(nums);
    median = s.find_median(nums);
    cout << "a unsorted vector's median = " << median << endl;
    return 0;
}