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


Source::Source(Config&& config)
    : impl{new Impl(std::move(config))}
{}

Source::Source(Source&& other)
    : impl{other.impl}
{
    other.impl = nullptr;
}

Source& Source::operator=(Source&& other)
{
    std::swap(impl, other.impl);
    return *this;
}

Source::~Source()
{
    delete impl;
}

void Source::add_random_event(const byte* data, byte data_size)
{
    impl->add_random_event(data, data_size);
}


} // namespace fortuna_daemon
