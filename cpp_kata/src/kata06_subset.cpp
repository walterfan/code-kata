#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using int_vec = vector<int>;
using int_2d_vec = vector<vector<int>>;


void backtrace(vector<int>& numbers, int start, vector<int>& track, int_2d_vec& res) {
    // reach the leaf node of decision tree
    res.push_back(track);

    for (int i=start; i < numbers.size(); ++i) {
        //select option: numbers[i]
        track.push_back(numbers[i]);
        //recursion
        backtrace(numbers, i + 1, track, res);
        //remove the option
        track.pop_back();

    }
}

void subsets(vector<int>& numbers, int_2d_vec& res) {
    if (numbers.empty()) {
        return;
    }

    vector<int> track;
    backtrace(numbers, 0, track, res);

}


int kata06_subset(int argc, char** argv) {

    vector<int> numbers{1,2,3,4};
    int_2d_vec res;
    subsets(numbers, res);
    cout << "subset result: " << res.size() << endl;
    int i = 0;
    for(auto& vec: res) {

        std::ostringstream oss;
        for(auto& num: vec) {
            oss << num << " ";
        }
        cout << ++i << ". " << oss.str() << endl;
    }
    return 0;
}