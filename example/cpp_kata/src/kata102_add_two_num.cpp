#include <cstring>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <string_view>
#include <memory>

using namespace std;

/*
You are given two non-empty linked lists representing two non-negative integers.

The digits are stored in reverse order, and each of their nodes contains a single digit.

Add the two numbers and return the sum as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example 1:
------------------
Input: l1 = [2,4,3], l2 = [5,6,4]
Output: [7,0,8]
Explanation: 342 + 465 = 807.

Example 2:
------------------
Input: l1 = [0], l2 = [0]
Output: [0]

Example 3:
------------------
Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
Output: [8,9,9,9,0,0,0,1]
 

Constraints:

The number of nodes in each linked list is in the range [1, 100].
0 <= Node.val <= 9
It is guaranteed that the list represents a number that does not have leading zeros.
*/


struct ListNode {
    int val;
    ListNode *next;

    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode head(0);
        ListNode* cur = &head;
        int carry = 0;
        while(l1 || l2 || carry){
            int x = l1 ? l1->val : 0;
            int y = l2 ? l2->val : 0;
            
            ListNode* node = new ListNode((x + y + carry) % 10);
            cur->next = node;
            cur = node;
            
            carry = (x + y + carry) / 10;
            
            if(l1) l1 = l1->next;
            if(l2) l2 = l2->next;
        }
        return head.next;
    }
};

int kata102_add_two_num(int argc, char** argv) {

    ListNode* l1 = new ListNode(2);
    l1->next = new ListNode(4);
    l1->next->next = new ListNode(3);

    ListNode* l2 = new ListNode(5);
    l2->next = new ListNode(6);
    l2->next->next = new ListNode(4);

    Solution s;
    ListNode* result = s.addTwoNumbers(l1, l2);

    cout << "Input: l1 = [2,4,3], l2 = [5,6,4]" << endl;
    cout << "Output: ";
    while(result) {
        cout << result->val << " ";
        result = result->next;
    }
    cout << endl;

    return 0;
}
