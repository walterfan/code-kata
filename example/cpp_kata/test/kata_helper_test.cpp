#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include "kata_helper.h"

using namespace std;
using namespace testing;

//using ::testing::Return;
//using ::testing::Mock;

bool is_sorted(const std::vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            return false; // If any element is smaller than the previous, it's not sorted
        }
    }
    return true; // All elements are in non-decreasing order
}

TEST(KataHelperTest, merge_sort)
{
	cout<<"--- merge_sort --"<<endl;
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};

    std::cout << "Given array: ";
    print_array(arr);

    merge_sort(arr, 0, arr.size() - 1);

    std::cout << "Sorted array: ";
    print_array(arr);
    ASSERT_TRUE(::is_sorted(arr));
}


TEST(KataHelperTest, quick_sort)
{
    cout<<"--- quick_sort --"<<endl;
    std::vector<int> arr = {10, 7, 8, 9, 1, 5};

    std::cout << "Given array: ";
    print_array(arr);

    quick_sort(arr, 0, arr.size() - 1);

    std::cout << "Sorted array: ";
    print_array(arr);

    ASSERT_TRUE(::is_sorted(arr));
}


TEST(KataHelperTest, insert_sort)
{
    cout<<"--- insert_sort --"<<endl;
    std::vector<int> arr = {12, 11, 13, 5, 6};

    std::cout << "Given array: ";
    print_array(arr);

    insert_sort(arr);

    std::cout << "Sorted array: ";
    print_array(arr);
    ASSERT_TRUE(::is_sorted(arr));
}

TEST(KataHelperTest, binary_search)
{
    cout<<"--- binary_search --"<<endl;
    std::vector<int> arr = {2, 3, 4, 10, 40};
    int target = 10;

    std::cout << "Given array: ";
    print_array(arr);

    int result = binary_search(arr, target);

    if (result != -1) {
        std::cout << "Element found at index " << result << std::endl;
    } else {
        std::cout << "Element not found in the array" << std::endl;
    }
    ASSERT_EQ(result, 3);
}