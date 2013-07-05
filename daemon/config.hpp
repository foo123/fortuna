/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna_daemon.

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

#ifndef FORTUNAD_CONFIG_HPP
#define FORTUNAD_CONFIG_HPP

#include <fortuna/accumulator.hpp>

#include "server.hpp"


struct Config
{
    fortuna::Accumulator::Config fortuna;
    Server::Config server;

    std::size_t threads_count {4};
};

#endif // FORTUNAD_CONFIG_HPP
