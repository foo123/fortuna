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
#include <string>
#include <utility>

#include <boost/program_options.hpp>


// http://stackoverflow.com/a/26263786
namespace boost {


template <>
std::string lexical_cast(const std::chrono::minutes& minutes)
{
    return std::to_string(minutes.count());
}

template <>
std::chrono::minutes lexical_cast(const std::string& string)
{
    return std::chrono::minutes{std::stoi(string)};
}

template <>
std::string lexical_cast(const std::chrono::milliseconds& milliseconds)
{
    return std::to_string(milliseconds.count());
}

template <>
std::chrono::milliseconds lexical_cast(const std::string& string)
{
    return std::chrono::milliseconds{std::stoi(string)};
}


} // namesapce boost


namespace fortuna_daemon {


namespace {


/**
 * Creates boost::program_options::value that is connected to given object
 * and its value is used as default.
 */
template <typename T>
auto self_default_value(T* val)
{
    return boost::program_options::value<T>(val)->default_value(*val);
}


} // namespace


Server::AllConfig handle_options(int argc, char* argv[])
{
    namespace po = boost::program_options;

    Server::AllConfig config;

    po::options_description options{"Options"};
    options.add_options()
        ("help,h", "print this help")
        ("socket,s", self_default_value(&config.server.connection_info.socket))
        ("min_pool_size", self_default_value(&config.accumulator.accumulator.min_pool_size), "minimum pool size [bytes]")
        ("generator_reseed_interval", self_default_value(&config.accumulator.generator.reseed_interval), "[milliseconds]")
        ("seed_file_path", self_default_value(&config.accumulator.seed_file_manager.seed_file_path))
        ("seed_file_length", self_default_value(&config.accumulator.seed_file_manager.seed_file_length), "[bytes]")
        ("seed_file_write_interval", self_default_value(&config.accumulator.seed_file_manager.write_interval), "[minutes]")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << options << std::endl;
        std::exit(0);
    }

    return config;
}


} // namespace fortuna_daemon
