#include <vector>

using namespace std;

vector<vector<int> subsets(vector<int>& numbers) {
    if (numbers.empty()) {
        return {{}};
    }

    int num = numbers.back();
    numbers.pop_back();

    vector<vector<int>> res = subsets(numbers);

    int size = res.size();
    for (int i = 0; i < size; ++i) {
        res.push_back(res[i]);
        res.back().push_back(n);
    }
    return res;
}


int kata05_subset(int argc, char** argv) {

    //TBD
    return 0;
}