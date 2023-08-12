#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/timer/timer.hpp>

using namespace std;

int fib1(int n) {
        if(n == 0) return 0;
        if(n == 1 || n == 2) return 1;
        return fib1(n-1) + fib1(n-2);
}


int helper(vector<int>& memo, int n) {
    if(n == 1 || n == 2) return 1;
    if(memo[n] != 0) return memo[n];
    memo[n] = helper(memo, n-1) + helper(memo, n-2);
    return memo[n];
}

int fib2(int N) {
    if(N == 0) return 0;
    if(N == 1 || N == 2) return 1;
    //initialize all elements in memo vector to 0
    vector<int> memo(N+1, 0);
    return helper(memo, N);
}

int fib3(int N) {
    if(N == 0) return 0;
    if(N == 1 || N == 2) return 1;

    vector<int> dp(N + 1, 0);
    //base case
    dp[1] = dp[2] = 1;
    for (int i = 3; i <= N; ++i) {
      dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[N];

}

int fib4(int N) {
    if(N == 0) return 0;
    if(N == 1 || N == 2) return 1;

    int prev = 1, curr = 1;
    for (int i = 3; i <= N; ++i) {
      int sum = prev + curr;
      prev = curr;
      curr = sum;
    }
    return curr;
}

int kata02_fib(int argc, char** argv) {
    int N = 40;
    BOOST_LOG_TRIVIAL(info)<<"* kata02_fib: compare 4 different implementation of Fibonacci sequence to " << N ;
    {
      boost::timer::auto_cpu_timer t;
      fib1(N);
    }
    {
      boost::timer::auto_cpu_timer t;
      fib2(N);
    }
    {
      boost::timer::auto_cpu_timer t;
      fib3(N);
    }
    {
      boost::timer::auto_cpu_timer t;
      fib4(N);
    }


    
    return 0;
}