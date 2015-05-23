/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_source_timer.

fortuna_source_timer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_source_timer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_source_timer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "options.hpp"

#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>


namespace fortuna_source_timer {


namespace
{

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


fortuna_daemon::Source::Config handle_options(int argc, char* argv[])
{
    namespace po = boost::program_options;

    fortuna_daemon::Source::Config config;
    unsigned short source_number; // passing byte to options does not work as numeric.

    po::options_description options("Options");
    options.add_options()
        ("help,h", "Print this help.")
        ("source_number,n", po::value(&source_number), "Required. Must be in range [0,255].")
        ("socket,s", self_default_value(&config.connection_info.socket), "Path to fortunad socket.")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: fortuna_source_timer [options]\n"
                  << '\n'
                  << options << std::endl;
        std::exit(0);
    }

    if (!vm.count("source_number"))
        throw std::invalid_argument("source number required");

    if (source_number > 255)
        throw std::invalid_argument("source_number > 255");

    config.source_number = source_number;
    return config;
}


} // namespace fortuna_source_timer
