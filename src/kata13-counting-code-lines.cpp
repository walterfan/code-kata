#include <string>
#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

class LineCounter {
public:
    virtual int countLines(const std::string& strFile) = 0;

    virtual ~LineCounter() {}
};

class LineCounterV1: LineCounter {
public:
    int countLines(const std::string& strFile) {
        int line_count = 0;

        std::ifstream in(strFile);
        char c = 0;
        while(in.get(c)) {
            if(c == '\n') {
                line_count++;
            }
        }
        return line_count;

    }
};

/*
Istreambuf iterators are input iterators that read successive elements from a stream buffer.

```
template <class InputIterator, class T>
  typename iterator_traits<InputIterator>::difference_type
    count (InputIterator first, InputIterator last, const T& val);
```
Count appearances of value in range

Returns the number of elements in the range [first,last) that compare equal to val.

The function uses operator== to compare the individual elements to val.

*/

class LineCounterV2: LineCounter {
public:
    int countLines(const std::string& strFile) {
        std::ifstream in(strFile);
        return std::count(
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>(),
            '\n'
        );

    }
};

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "please input file name" << endl;
    }

    auto lineCounter = make_unique<LineCounterV2>();
    int lineCount = lineCounter->countLines(argv[1]);
    cout << "line count is " << lineCount << endl;

}