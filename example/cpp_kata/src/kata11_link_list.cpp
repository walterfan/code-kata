
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>
#include "kata_helper.h"
using namespace std;

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* nextTemp = curr->next; // 保存下一个节点
        curr->next = prev;               // 当前节点的next指向前一个节点
        prev = curr;                     // 前一个节点前移
        curr = nextTemp;                 // 当前节点前移
    }
    
    return prev; // 返回新的头节点
}

ListNode* reverseListRecursive(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    ListNode* newHead = reverseListRecursive(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}


ListNode* reverseBetween(ListNode* head, int m, int n) {
    if (!head || m == n) return head;

    ListNode dummy(0);  // 创建一个虚拟头节点
    dummy.next = head;
    ListNode* prev = &dummy;  // prev 指向虚拟头节点

    // 将 prev 指向第 m 个节点的前一个节点
    for (int i = 1; i < m; ++i) {
        prev = prev->next;
    }

    // start 指向第 m 个节点，then 指向第 m+1 个节点
    ListNode* start = prev->next;
    ListNode* then = start->next;

    // 反转从第 m 到第 n 个节点
    for (int i = 0; i < n - m; ++i) {
        start->next = then->next;  // 断开 start 和 then 的连接
        then->next = prev->next;   // 将 then 插入到 prev 和 start 之间
        prev->next = then;         // prev 指向新的第一个节点 then
        then = start->next;        // 移动 then 指针到下一个节点
    }

    return dummy.next;
}

int kata11_link_list(int argc, char** argv) {
    // 创建链表 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);
    
    std::cout << "原链表: ";
    print_list(head);

    // 反转链表
    ListNode* reversedHead = reverseList(head);

    std::cout << "反转后的链表: ";
    print_list(reversedHead);
    
    // 释放内存（实际应用中要注意内存管理，这里只是演示）
    ListNode* curr = reversedHead;
    while (curr != nullptr) {
        ListNode* next = curr->next;
        delete curr;
        curr = next;
    }

    return 0;
}



