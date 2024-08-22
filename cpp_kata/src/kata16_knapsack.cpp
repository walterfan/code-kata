
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>
#include "kata_helper.h"
using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>

int knapsack(int W, const std::vector<int>& weights, const std::vector<int>& values, int n) {
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));

    // 填充dp数组, i 为物品个数, j 为背包容量, 值为最大价值,
    // 这个 dp 数组是一个二维数组, 第一维是物品个数, 第二维是背包容量, 值为最大价值
    // 注意它是一个状态矩阵, 和 weights 数组和 values 数组不一样, 不是从 0 开始的
    // dp[i][j] 表示前 i 个物品在容量为 j 时的最大价值
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= W; ++j) {
            //注意, weights 数组是从 0 开始的, weights[i-1] 就代表第 i 个物品的重量, values[i-1] 就代表第 i 个物品的价值
            if (weights[i - 1] <= j) {
                //我们求的是最大价值, 所以我们要比较不装进背包的价值和装进背包的价值哪个大
                dp[i][j] = std::max(dp[i - 1][j], //不装进背包的价值
                    dp[i - 1][j - weights[i - 1]] + values[i - 1]); //装进背包的价值
            } else {
                //如果不选择第 i 个物品，则最大价值为前 i - 1 个物品在容量为 j 时的最大价值
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    return dp[n][W];
}

int kata16_knapsack(int argc, char** argv) {
    int W = 50; // 背包容量
    std::vector<int> weights = {10, 20, 30}; // 物品重量
    std::vector<int> values = {60, 100, 120}; // 物品价值
    int n = weights.size(); // 物品数量

    std::cout << "最大价值: " << knapsack(W, weights, values, n) << std::endl;

    return 0;
}
