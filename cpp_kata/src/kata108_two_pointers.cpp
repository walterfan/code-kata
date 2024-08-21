#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>
#include "kata_helper.h"
using namespace std;

/*

*/
class Solution {
public:
    bool has_cycle(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast!= NULL && fast->next!= NULL) {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow) {
                return true;
            }
        }
        return false;
    }

    ListNode* detect_cycle(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast!= NULL && fast->next!= NULL) {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow) {
                break;
            }
        }
        if (fast == NULL || fast->next == NULL) {
            return NULL;
        }
        fast = head;
        while (fast!= slow) {
            fast = fast->next;
            slow = slow->next;
        }
        return fast;
    }

    ListNode* find_middle_point(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast!= NULL && fast->next!= NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    ListNode* find_last_n_point(ListNode* head, int n) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast!= NULL && n > 0) {
            fast = fast->next;
            n--;
        }
        while (fast!= NULL) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

int kata108_two_pointers(int argc, char** argv) {

    ListNode* head = new ListNode(1);
    vector<int> nums = {1,2,3,4,5,6};
    ListNode* node_3 = nullptr;
    for(int i=1; i < nums.size(); i++) {
        head->next = new ListNode(nums[i]);
        head = head->next;
        if (head->val == 3) {
            node_3 = head;
        } else if (head->val == 6) {
            head->next = node_3;
        }
    }

    Solution s;
    ListNode* cycle_node = s.detect_cycle(head);
    cout << "cycle node = " << cycle_node->val << endl;

    vector<int> nums1 = {1,2,3,4,5,6};
    ListNode* sample_list = build_list(nums1);

    ListNode* middle_node = s.find_middle_point(sample_list);
    cout << "middle node = " << middle_node->val << endl;

    return 0;
}
