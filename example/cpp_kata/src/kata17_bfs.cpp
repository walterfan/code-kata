
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>
#include "kata_helper.h"
using namespace std;

vector<char> bfs(unordered_map<char, vector<char>>& graph, char start_node) {
    unordered_set<char> visited; // 用于跟踪已访问的节点
    queue<char> q;               // 使用队列来存储需要访问的节点顺序
    vector<char> result;         // 用于保存遍历顺序

    q.push(start_node);          // 从起始节点开始
    visited.insert(start_node);  // 标记起始节点为已访问

    while (!q.empty()) {
        char node = q.front();   // 访问队列中的第一个节点
        q.pop();
        result.push_back(node);  // 将该节点添加到结果中

        // 将该节点的未访问邻居添加到队列中
        for (char neighbor : graph[node]) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }

    return result;
}

// 使用DFS算法来求解二叉树的最小高度
int minDepthDfs(TreeNode* root) {
    if (!root) return 0; // 空节点的高度为0

    // 如果一个子树为空，则返回另一个子树的最小高度 + 1
    if (!root->left) return minDepthDfs(root->right) + 1;
    if (!root->right) return minDepthDfs(root->left) + 1;

    // 如果两个子树都存在，则返回两者的最小高度 + 1
    return std::min(minDepthDfs(root->left), minDepthDfs(root->right)) + 1;
}

// 使用BFS算法来求解二叉树的最小高度
int minDepthBfs(TreeNode* root) {
    if (!root) return 0; // 空节点的高度为0

    std::queue<TreeNode*> q;
    q.push(root);
    int depth = 1;

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();

            // 当遇到叶子节点时，直接返回当前深度
            if (!node->left && !node->right) {
                return depth;
            }

            // 将左右子节点加入队列
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ++depth; // 每遍历完一层，深度加1
    }

    return depth;
}

int test_graph_bfs() {
    // 定义图
    unordered_map<char, vector<char>> graph = {
        {'A', {'B', 'C'}},
        {'B', {'A', 'D', 'E'}},
        {'C', {'A', 'F'}},
        {'D', {'B'}},
        {'E', {'B', 'F'}},
        {'F', {'C', 'E'}}
    };

    // 从节点 'A' 开始运行 BFS
    vector<char> result = bfs(graph, 'A');

    // 打印结果
    cout << "BFS 结果: ";
    for (char node : result) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}

int kata17_bfs(int argc, char** argv) {
    // 构建一个二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    std::cout << "二叉树的最小高度是: " << minDepthBfs(root) << std::endl;

    // 释放内存
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}




