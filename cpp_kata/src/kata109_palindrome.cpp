#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>
#include "kata_helper.h"
using namespace std;


class Solution {
public:
    bool isPalindrome(const std::string& str, int left, int right) {
        //int left = 0;
        //int right = str.size() - 1;
        while (left < right) {
            if (str[left]!= str[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }

    bool validPalindrome(const std::string& str) {
        int left = 0;
        int right = str.size() - 1;
        while (left < right) {
            if (str[left]!= str[right]) {
                return isPalindrome(str, left + 1, right) || isPalindrome(str, left, right - 1);
            }
            left++;
            right--;
        }
        return true;
    }

    // Function to reverse the linked list
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* current = head;
        ListNode* next = nullptr;

        while (current != nullptr) {
            next = current->next; // Store the next node
            current->next = prev; // Reverse the current node's pointer
            prev = current;       // Move prev to this node
            current = next;       // Move to the next node
        }
        head = prev; // Update the head to the new front
        return head;
    }


    bool isPalindrome(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* next = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;

        //reverse the first half of the list, and find the middle point
        while (fast && fast->next) {
            fast = fast->next->next;
            next = slow->next;
            slow->next = prev;
            prev = slow;
            slow = next;

        }

        if(fast) {
            slow = slow->next;
        }

        //slow point to the middle point now
        while (slow) {
            if (slow->val != prev->val) {
                return false;
            }
            prev = prev->next;
            slow = slow->next;
        }
        return true;
    }
};

int kata109_palindrome(int argc, char** argv) {
    string str = "abcba";
    Solution s;
    bool result = s.isPalindrome(str, 0, str.size() - 1);
    cout << str << " is palindrome? " << result << endl;

    str = "abca";
    result = s.validPalindrome(str);
    cout << str << " is palindrome after remove one char? " << result << endl;

    vector<int> vec = {1, 2, 3, 4, 3, 2, 1};
    ListNode* head = build_list(vec);
    print_list(head);
    result = s.isPalindrome(head);
    cout << "is palindrome? " << result << endl;

    return 0;
}
