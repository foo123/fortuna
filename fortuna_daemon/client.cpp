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

#include "client.hpp"

#include <utility>

#include "client_impl.hpp"


namespace fortuna_daemon {


const std::size_t fortuna_daemon::Client::max_request_length;

Client::Client(Config&& config)
    : impl(new Impl(std::move(config)))
{}

Client::~Client()
{
    delete impl;
}

void Client::get_random_data(byte* data, std::size_t length)
{
    impl->get_random_data(data, length);
}


} // namespace fortuna_daemon
