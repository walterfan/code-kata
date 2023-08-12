#include "kata_runner.h"
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time

using namespace std;
namespace po = boost::program_options;

static std::string current_time()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

#define DECLARE_KATA(name) extern int name(int argc, char** argv)
#define REGISTER_KATA(name) register_kata(#name, name)

DECLARE_KATA(kata01_buffer);
DECLARE_KATA(kata02_fib);
DECLARE_KATA(kata03_crtp);
DECLARE_KATA(kata04_logger);
DECLARE_KATA(kata13_line_counter);


const char* usage = R"name(please specify kata name:
e.g. ./bin/kata_runner --name kata...
)name";



//----------------------- KataRunner ------------------------------//
KataRunner::KataRunner(): m_kata_count(0),m_katas() {
    BOOST_LOG_TRIVIAL(trace)<<"* KataRunner construct: " ;
}

KataRunner::~KataRunner() {
    BOOST_LOG_TRIVIAL(trace)<<"* KataRunner destruct: ";
}

void KataRunner::init() {
    REGISTER_KATA(kata01_buffer);
    REGISTER_KATA(kata02_fib);
    REGISTER_KATA(kata03_crtp);
    REGISTER_KATA(kata04_logger);
    REGISTER_KATA(kata13_line_counter);
}

void KataRunner::register_kata(const string& name, const exam_func_t &exam)
{
    m_kata_count++;
    m_katas[name] = exam;
    m_commands[m_kata_count] = name;
}

int KataRunner::execute_kata(const string& name, int argc, char** argv) const
{
    auto it = m_katas.find(name);
    if(it != m_katas.end()) {
        BOOST_LOG_TRIVIAL(trace) << "execute "<< it->first;
        exam_func_t func = it->second;
        return func(argc, argv);
    }
    BOOST_LOG_TRIVIAL(trace) << "not registered "<< name;
    return -1;
}

kata_func_ptr KataRunner::find_kata(int kataId) const {
    auto it = m_commands.find(kataId);
    if(it != m_commands.end()) {
        return find_kata(it->second);
    }
    return nullptr;
}

kata_func_ptr KataRunner::find_kata(const std::string& kataName) const {

    auto it = m_katas.find(kataName);
    if(it != m_katas.end()) {
        return it->second;
    }
    return nullptr;
}


size_t KataRunner::size() const {
    return m_katas.size();
}

void KataRunner::display_menu() const {
    cout<<"===================================================== \n";
    cout<<" \t\tMENU \t \n ";
    cout<<"===================================================== \n";


    for(auto&& [first,second]: m_commands) {
        cout << " " << first << ". " << second << endl;
    }
    cout<<" 0. quit\n";

}

int main(int argc, char** argv)
{
    unique_ptr<KataRunner> runner = make_unique<KataRunner>();
    runner->init();

    //c++11 R"raw string"
    po::options_description desc("Allowed options:");

    desc.add_options()
        ("help,h", "produce help message")
        ("name,n", po::value<string>(), usage)
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        BOOST_LOG_TRIVIAL(trace) << desc << "\n";
        return 1;
    }

    boost::log::core::get()->set_filter (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    if (vm.count("name")) {
        BOOST_LOG_TRIVIAL(trace) << "* kata name is "<< vm["name"].as<string>() << ".";
        runner->execute_kata(vm["name"].as<string>(), argc, argv);
    } else {
        runner->display_menu();
        int nChoice = 0;
        do {
            cout<<"Enter your choice(0-" << runner->size() << "):";
            cin>>nChoice;

            auto func_ptr = runner->find_kata(nChoice);
            if(func_ptr) {
                cout << "# start at " << current_time() << endl;
                func_ptr(argc, argv);
                cout << "# end at " << current_time() << endl;
            } else {
                cout<<"Invalid option, please select again"<<endl; runner->display_menu();
            }

        } while (nChoice > 0);
    }

    return 0;
}
