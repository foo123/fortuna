/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

This file is part of fortuna.

libfortuna_daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libfortuna_daemon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libfortuna_daemon.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "source.hpp"

#include <utility>

#include "source_impl.hpp"


namespace fortuna_daemon {


const std::size_t Source::max_event_length;


Source::Source(const Config& config)
    : impl{new Impl{config}}
{}

Source::~Source() noexcept
{}


void Source::add_random_event(const byte* data, byte data_size)
{
    impl->add_random_event(data, data_size);
}

void Source::add_random_events(const byte* data, std::size_t length)
{
    std::size_t data_left_to_add = length;
    while (data_left_to_add) {
        std::size_t data_to_add_now = std::min(max_event_length, data_left_to_add);
        impl->add_random_event(data, data_to_add_now);
        data_left_to_add -= data_to_add_now;
        data += data_to_add_now;
    }
}


} // namespace fortuna_daemon
