/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_source_file.

fortuna_source_file is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fortuna_source_file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fortuna_source_file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "options.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>

//#include "main.hpp"


namespace fortuna_source_file {


namespace
{

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


Application::AllConfig handle_options(int argc, char* argv[])
{
    namespace po = boost::program_options;

    Application::AllConfig config;
    unsigned short source_number; // passing byte to options does not work as numeric.

    po::options_description public_options("Options");
    public_options.add_options()
        ("help,h", "Print this help.")
        ("source_number,n", po::value(&source_number), "Required. Must be in range [0,255].")
        ("socket,s", self_default_value(&config.fortuna_source.connection_info.socket), "Path to fortunad socket.")
    ;

    po::options_description all_options;
    all_options.add(public_options);
    all_options.add_options()
        ("files,f", po::value(&config.app.files), "input files")
    ;

    po::positional_options_description positional_options;
    positional_options.add("files", -1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv)
            .options(all_options)
            .positional(positional_options)
            .run(),
        vm
    );
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: fortuna_source_file [options] [--] [file...]\n"
                  << '\n'
                  << "With no file, or when file is -, read standard input.\n"
                  << '\n'
                  << public_options << std::endl;
        std::exit(0);
    }

    if (!vm.count("source_number"))
        throw std::invalid_argument("source number required");

    if (source_number > 255)
        throw std::invalid_argument("source_number > 255");

    if (std::count(config.app.files.cbegin(), config.app.files.cend(), "-") > 1)
        throw std::invalid_argument("too many \"-\"");

    if (config.app.files.size() == 0)
        config.app.files.push_back("-");

    config.fortuna_source.source_number = source_number;
    return config;
}


} // namespace fortuna_source_file
