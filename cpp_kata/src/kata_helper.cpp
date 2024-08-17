#include "kata_helper.h"
#include <queue>

void print_tree(TreeNode* root, std::ostream& out, TraversalOrder order) {
    if (root == nullptr) {
        std::cout << "null" << std::endl;
        return;
    }
    if (order == TraversalOrder::PRE_ORDER) {
        out << root->val << " ";
        print_tree(root->left, out, order);
        print_tree(root->right, out, order);
    } else if (order == TraversalOrder::IN_ORDER) {
        print_tree(root->left, out, order);
        out << root->val << " ";
        print_tree(root->right, out, order);
    } else if (order == TraversalOrder::POST_ORDER) {
        print_tree(root->left, out, order);
        print_tree(root->right, out, order);
        out << root->val << " ";
    } else if (order == TraversalOrder::LEVEL_ORDER) {
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            out << node->val << " ";
            if (node->left!= nullptr) {
                q.push(node->left);
            }
            if (node->right!= nullptr) {
                q.push(node->right);
            }
        }
    }

}


void print_array(const std::vector<int>& nums, std::ostream& out ) {
    for (auto num : nums) {
        out << num << " ";
    }
    out << std::endl;
}

void print_list(ListNode* head, std::ostream& out) {
    while (head!= nullptr) {
        out << head->val << " ";
        head = head->next;
    }
    out << std::endl;
}