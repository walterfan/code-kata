#ifndef KATA_RUNNER_H_
#define KATA_RUNNER_H_

#include <stdio.h>
#include <stdint.h>

#include <string>
#include <map>
#include <iostream>
#include <memory>

#include <unordered_map>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <boost/program_options.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/assert.hpp>

#ifndef DEBUG_TRACE
#define DEBUG_TRACE(msg) do { \
            std::cout <<"["<<time(NULL)<<","<< __FILE__ << "," << __LINE__ << "]\t"<< msg << std::endl; \
        } while(0)
#endif

//old function pointer
//typedef int (*kata_func_ptr)(int argc, char** argv);
//new function object
typedef std::function<int(int, char**)> exam_func_t;

using kata_func_ptr = std::function<int(int, char**)>;

class KataRunner: boost::noncopyable {
public:
    KataRunner();
    //no need by noncopyable
    //KataRunner(const KataRunner& rhs);
    //KataRunner& operator=(const KataRunner& rhs);

    virtual ~KataRunner();

    void init();

    size_t size() const;

    void register_kata(const std::string& name, const exam_func_t &exam);

    int execute_kata(const std::string& name, int argc, char** argv) const;

    kata_func_ptr find_kata(int kataId) const;

    kata_func_ptr find_kata(const std::string&kataId) const;

    void display_menu() const;
private:
    uint32_t m_kata_count = 0;
    std::unordered_map<std::string, exam_func_t> m_katas;
    std::map<int, std::string> m_commands;
};


#endif