/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

fortuna_daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_daemon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_daemon.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "options.hpp"

#include <iostream>
#include <utility>

#include <boost/program_options.hpp>


namespace fortuna_daemon {


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


Application::AllConfig handle_options(int argc, char* argv[], Application::AllConfig&& config)
{
    namespace po = boost::program_options;
    
    po::options_description options("General Options");
    options.add_options()
        ("help,h", "print this help")
        ("socket,s", self_default_value(&config.server.connection_info.socket))
        ("threads,t", "specify number of worker threads (defaults to number of hardware threads)")
    ;
    
    po::options_description accumulator_options("Accumulator Options");
    accumulator_options.add_options()
        ("min_pool_size,p", self_default_value(&config.accumulator.min_pool_size), "minimum pool size [bytes]")
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
    
    if (vm.count("threads")) {
        config.application.default_threads_num = false;
        config.application.custom_threads_num = vm["threads"].as<decltype(config.application.custom_threads_num)>();
    }
    
    return std::move(config);
}


} // namespace fortuna_daemon
