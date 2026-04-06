
#include <iostream>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

void dfs(TreeNode* node, std::vector<int>& path, std::vector<std::string>& paths) {
    if (!node) return;

    // Add the current node's value to the path
    path.push_back(node->val);

    // If it's a leaf node, convert the path to a string and add to paths vector
    if (!node->left && !node->right) {
        std::string leafPath;
        for (int i = 0; i < path.size(); ++i) {
            //if (i > 0) leafPath += "->";
            leafPath += std::to_string(path[i]);
        }
        paths.push_back(leafPath);
    }

    // Recursively traverse the left and right subtrees
    dfs(node->left, path, paths);
    dfs(node->right, path, paths);

    // Backtrack: remove the current node's value from the path
    path.pop_back();
}

std::vector<std::string> getRootToLeafPaths(TreeNode* root) {
    std::vector<std::string> paths;
    std::vector<int> path;
    dfs(root, path, paths);
    return paths;
}

TreeNode* createTree(const std::vector<int>& values, int index) {
    if (index >= values.size() || values[index] == -1) {
        return nullptr;
    }
    TreeNode* node = new TreeNode(values[index]);
    node->left = createTree(values, 2 * index + 1);
    node->right = createTree(values, 2 * index + 2);
    return node;
}

void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int kata12_tree_path_sum(int argc, char** argv) {
    // Example usage:
    // Creating a binary tree with the following structure:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    std::vector<int> values = {1, 2, 3, 4, 5, -1, -1};
    TreeNode* root = createTree(values, 0);

    // Get all root-to-leaf paths
    std::vector<std::string> paths = getRootToLeafPaths(root);

    int sum = 0;
    for (const auto& path : paths) {
        std::cout << path << std::endl;
        sum += atoi(path.c_str());
    }
    cout << "sum=" << sum << endl;
   // Clean up the tree to prevent memory leaks
    deleteTree(root);

    return 0;
}







