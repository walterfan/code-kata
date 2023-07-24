#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include <string_view>
#include <memory>
#include <map>

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>


std::map<std::string, std::function<void()>> commands;

template<typename ftor>
void install_command(std::string name, ftor && handler)
{
  commands.insert({
    std::move(name),
    std::forward<ftor>(handler)
  });
}


int kata02_move(int argc, char** argv) {


    return 0;
}