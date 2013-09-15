#include "options.hpp"

#include <iostream>

#include <boost/program_options.hpp>

namespace {

/**
 * Creates boost::program_options::value that is connected to given object
 * and its value is used as default.
 */
template <typename T>
boost::program_options::typed_value<T>* self_default_value(T* val)
{
    return boost::program_options::value<T>(val)->default_value(*val);
}

} // namespace

Server::Config handle_options(int argc, char* argv[], Server::Config&& config)
{
    namespace po = boost::program_options;
    
    po::options_description options("General Options");
    options.add_options()
        ("help,h", "print this help")
        ("socket,s", self_default_value<std::string>(&config.socket))
    ;
    
    po::options_description accumulator_options("Accumulator Options");
    accumulator_options.add_options()
        ("min_pool_size", self_default_value<unsigned long>(&config.fortuna.min_pool_size), "minimum pool size [bytes]")
    ;
    
    options
        .add(accumulator_options)
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        std::cout << options << std::endl;
        std::exit(0);
    }
    
    return std::move(config);
}
