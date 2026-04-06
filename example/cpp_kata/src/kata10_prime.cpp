
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>
#include "kata_helper.h"
using namespace std;

//素数是指只能被1和自身整除的正整数
// 例如，2、3、5、7、11 等都是素数，而 4、6、8、9 等不是素数。

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}


std::vector<int> sieveOfEratosthenes(int m, int n) {
    std::vector<bool> is_prime(n + 1, true);  // 初始化为true
    std::vector<int> primes;

    is_prime[0] = is_prime[1] = false; // 0 和 1 不是素数

    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;  // 标记所有 p 的倍数为非素数
            }
        }
    }

    // 收集所有 [m, n] 范围内的素数
    for (int i = std::max(m, 2); i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}




int kata10_prime(int argc, char** argv) {

    int m = 10, n = 50; // 范围 [10, 50]

    std::vector<int> primes = sieveOfEratosthenes(m, n);

    std::cout << "范围 [" << m << ", " << n << "] 内的素数是:" << std::endl;
    for (int prime : primes) {
        std::cout << prime << " ";
    }

    return 0;

}




