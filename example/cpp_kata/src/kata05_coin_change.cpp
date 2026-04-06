#include <vector>

using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

int coinChange(const std::vector<int>& coins, int amount) {
    // 初始化dp数组，大小为amount + 1，初始值为amount + 1（表示无法凑出）
    // dp[i]表示凑出金额 i 所需的最少硬币数
    std::vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0; // 金额为0时需要0个硬币

    // 动态规划求解
    for (int i = 1; i <= amount; ++i) {
        for (const int& coin : coins) {
            if (i - coin >= 0) {
                dp[i] = std::min(dp[i], // 不选择当前硬币, dp[i] 的值不变
                    dp[i - coin] + 1); // 选择当前硬币, dp[i] 的值更新为 dp[i - coin] + 1
            }
        }
    }

    // 如果dp[amount] 仍然是amount + 1，说明无法凑出该金额
    return dp[amount] > amount ? -1 : dp[amount];
}


int kata05_coin_change(int argc, char** argv) {

    std::vector<int> coins = {1, 2, 5};
    int amount = 11;

    int result = coinChange(coins, amount);

    if (result != -1) {
        std::cout << "凑出金额 " << amount << " 所需的最少硬币数是: " << result << std::endl;
    } else {
        std::cout << "无法凑出金额 " << amount << std::endl;
    }
    return 0;
}