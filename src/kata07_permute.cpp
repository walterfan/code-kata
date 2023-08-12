#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using int_vec = vector<int>;
using int_2d_vec = vector<vector<int>>;

void bracktrace(vector<int>& numbers, vector<int>& track, int_2d_vec& res) {
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
        bracktrace(numbers, track, res);
        //remove the option -- the i_th number
        track.pop_back();

    }
}

void permute(vector<int>& numbers, int_2d_vec& res) {
    if (numbers.empty()) {
        return;
    }

    vector<int> track;
    bracktrace(numbers, track, res);
}

int kata07_permute(int argc, char** argv) {
    vector<int> numbers{1,2,3,4};
    int_2d_vec res;
    permute(numbers, res);
    cout << "permute result: " << res.size() << endl;
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