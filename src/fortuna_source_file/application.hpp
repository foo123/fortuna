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

#ifndef FORTUNA_SOURCE_FILE_APPLICATION_HPP
#define FORTUNA_SOURCE_FILE_APPLICATION_HPP

#include <vector>
#include <string>

#include <libfortuna_daemon/source.hpp>


namespace fortuna_source_file {


class Application
{
public:
    struct Config
    {
        std::vector<std::string> files;

        Config()
        {}
    };

    struct AllConfig
    {
        Config app;
        fortuna_daemon::Source::Config fortuna_source;

        AllConfig()
        {}
    };

private:
    const Config config;
    fortuna_daemon::Source fortuna;

public:
    Application(AllConfig all_config = AllConfig{})
        : config{all_config.app}
        , fortuna{all_config.fortuna_source}
    {}

    void run();
};


} // namespace fortuna_source_file

#endif // FORTUNA_SOURCE_FILE_APPLICATION_HPP
