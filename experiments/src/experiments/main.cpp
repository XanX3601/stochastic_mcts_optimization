#include <cxxopts.hpp>
#include <iostream>

int main(int argc, const char** argv) {
    cxxopts::Options options("experiments", "Experiments launcher for stochastic mcts optimization");

    options.add_options()
        ("b,bar", "Param bar", cxxopts::value<std::string>())
        ("h,help", "Print help")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
}