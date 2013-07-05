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

Application::Config handle_options(int argc, char* argv[], Application::Config&& config)
{
    namespace po = boost::program_options;
    
    po::options_description options("General Options");
    options.add_options()
        ("help,h", "print this help")
        ("notify-ready,n", "print \"ready\" to stdout when ready for incomming connections")
        ("port,p", self_default_value<unsigned short>(&config.server.port), "tcp port number for incomming connections")
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
    
    if (vm.count("notify-ready"))
        config.notify_ready = true;
    
    return std::move(config);
}
