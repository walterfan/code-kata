#include <vector>

using namespace std;

int coin_change(vector<int>& coins, int amount) {
    vector<int> dp(amount +1, amount + 1);
    dp[0] = 0;
    // traverse all states
    for (int i = 0; i < dp.size(); ++i) {
        // traverse all options for min value
        for (int coin: coins) {
            //invalid case, ignore
            if (i - coin < 0) continue;
            dp[i] = min(dp[i], 1 + dp[i -coin]);
        }
    }
    return dp[amount] == amount + 1? -1 : dp[amount];
}


int kata05_coin_change(int argc, char** argv) {

    //TBD
    return 0;
}