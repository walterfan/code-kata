#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using int_vec = vector<int>;
using int_2d_vec = vector<vector<int>>;

/*
def backtracking(current_option):
   if satisfied:
     return solution
   if not satisfied:
     return null
   next_option = getNext()
   backtrack(next_option)
   undo next_option

1. 画出递归树，找到状态变量(回溯函数的参数)，这一步非常重要※
2. 根据题意，确立结束条件
3. 找准选择列表(与函数参数相关)，与第一步紧密关联※
4. 判断是否需要剪枝
5. 作出选择，递归调用，进入下一层
6. 撤销选择


决策一个回溯问题，实际上就是解决一个决策树的遍历过程。需要考虑这三个问题：

* 已走路径：已做出选择，走过的路径
* 可选列表：你当前可以做的选择
* 结束条件：一般走到决策树的叶子节点，它无法再做别的条件选择

*/

void backtrace(vector<int>& numbers, vector<int>& track, int_2d_vec& res) {
    // reach the leaf node of decision tree
    if (track.size() == numbers.size()) {
        res.push_back(track);
        return;
    }

    for (int i=0; i < numbers.size(); ++i) {
        //exclude illegal option
        std::vector<int>::iterator it;
        it = find (track.begin(), track.end(), numbers[i]);
        if (it != track.end()) continue;

        //select the option -- the i_th number
        track.push_back(numbers[i]);
        //go to the decision tree - rest numbers
        backtrace(numbers, track, res);
        //remove the option -- the i_th number
        track.pop_back();

    }
}

void permute(vector<int>& numbers, int_2d_vec& res) {
    if (numbers.empty()) {
        return;
    }

    vector<int> track;
    backtrace(numbers, track, res);
}

void print_2d_vec(int_2d_vec& res) {
    cout << "matrix size: " << res.size() << endl;
    int i = 0;
    for(auto& vec: res) {

        std::ostringstream oss;
        for(auto& num: vec) {
            oss << num << " ";
        }
        cout << ++i << ". " << oss.str() << endl;
    }
}

int kata07_permute(int argc, char** argv) {
    vector<int> numbers{1,2,3,4};
    int_2d_vec res;
    permute(numbers, res);
    print_2d_vec(res);
    return 0;
}