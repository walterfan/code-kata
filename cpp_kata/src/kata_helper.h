#ifndef KATA_HELPER_H_
#define KATA_HELPER_H_
/**
 * A helper class. A more elaborate class description.
 */
#include <vector>
#include <iostream>


class ListNode {
public:
    int val;
    ListNode* next;
};

template<typename Comparable>
class BinarySearchTree {
public:
    BinarySearchTree();
    BinarySearchTree(const std::vector<Comparable>& items);
    BinarySearchTree(const BinarySearchTree& rhs);
    ~BinarySearchTree();

private:
    struct BinaryNode {
        Comparable data;
        BinaryNode *left;
        BinaryNode *right;
        BinaryNode(const Comparable& theData) : data(theData), left(nullptr), right(nullptr) { }
    };
    BinaryNode* root;
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

enum class TraversalOrder {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    LEVEL_ORDER
};

void print_tree(TreeNode* root, std::ostream& out = std::cout, TraversalOrder order=TraversalOrder::PRE_ORDER);
void print_array(const std::vector<int>& nums, std::ostream& out = std::cout);
void print_list(ListNode* head, std::ostream& out = std::cout);

#endif