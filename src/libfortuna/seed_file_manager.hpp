/*
Copyright 2015 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

libfortuna is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libfortuna is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libfortuna.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FORTUNA_SEED_FILE_MANAGER_HPP
#define FORTUNA_SEED_FILE_MANAGER_HPP

#include <chrono>
#include <string>

#include "repeating_task.hpp"

namespace fortuna {

class Accumulator;


class SeedFileManager
{
public:
    struct Config
    {
        std::string seed_file_path = "./fortuna.seed";
        std::size_t seed_file_length = 64;
        std::chrono::minutes write_interval{10};

        Config()
        {}
    };

private:
    const Config config;
    Accumulator& accumulator;
    RepeatingTask repeating_task;

public:
    SeedFileManager(Config _config, Accumulator& _accumulator);

private:
    void write_seed_file();
    void update_seed_file();
};


} // namespace fortuna

#endif // FORTUNA_SEED_FILE_MANAGER_HPP
