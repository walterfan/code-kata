#ifndef KATA_HELPER_H_
#define KATA_HELPER_H_
/**
 * A helper class for kata
 *
 */
#include <vector>
#include <list>
#include <iostream>
#include <climits>

class ListNode {
public:
    int val;
    ListNode* next;

    ListNode(int x) : val(x), next(NULL) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
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


class Stack {
public:
    Stack(int size) {
        arr = new int[size];
        capacity = size;
        top = -1;
    }

    ~Stack() {
        delete[] arr;
    }

    bool push(int x);
    int pop();
    int peek();
    bool isEmpty();
    bool isFull();
private:
    int* arr;
    int top;
    int capacity;
};

class Queue {
public:
    Queue(int cap) {
        arr = new int[cap];
        capacity = cap;
        front = size = 0;
        back = capacity - 1;
    }

    ~Queue() {
        delete[] arr;
    }

    bool enqueue(int x);
    int dequeue();
    int peekFront();
    int peekBack();
    bool isEmpty();
    bool isFull();
private:
    int* arr;
    int front, back, size, capacity;
};


class BinaryTree {
public:
    TreeNode* root;

    // Constructor to initialize the binary tree
    BinaryTree() {
        root = nullptr;
    }

    void insert(int data);
    void inorderTraversal(TreeNode* node);
    void preorderTraversal(TreeNode* node);
    void postorderTraversal(TreeNode* node);
};

class Graph {
public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int v, int w);

    // Function to perform DFS traversal from a given vertex
    void DFS(int v);

    // Function to perform BFS traversal from a given vertex
    void BFS(int v);
private:
    int V; // Number of vertices
    std::vector<std::list<int>> adj; // Pointer to an array containing adjacency lists

};


void print_tree(TreeNode* root, std::ostream& out = std::cout, TraversalOrder order=TraversalOrder::PRE_ORDER);

void print_array(const std::vector<int>& nums, std::ostream& out = std::cout);

void print_list(ListNode* head, std::ostream& out = std::cout);

ListNode* build_list(std::vector<int>& nums);

void merge_sort(std::vector<int>& nums, int left, int right);

void quick_sort(std::vector<int>& arr, int low, int high);

void insert_sort(std::vector<int>& arr);

int binary_search(const std::vector<int>& arr, int target);

#endif