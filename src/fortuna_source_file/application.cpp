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

#include <iostream>
#include <fstream>

#include <cryptopp/secblock.h>

#include "application.hpp"


namespace fortuna_source_file {

namespace {

void read_stream_to_fortuna(std::istream& stream, fortuna_daemon::Source& fortuna)
{
    CryptoPP::SecByteBlock buffer{32};
    
    do {
        stream.read(reinterpret_cast<char*>(buffer.BytePtr()), buffer.SizeInBytes());
        std::streamsize gcount = stream.gcount();
        if (gcount > 0)
            fortuna.add_random_event(buffer.BytePtr(), gcount);
    } while (stream.good());
}

} // namespace


void Application::run()
{
    for (auto filename : config.files) {
        if (filename == "-") {
            read_stream_to_fortuna(std::cin, fortuna);
        } else {
            std::ifstream filestream(filename, std::ios::binary);
            read_stream_to_fortuna(filestream, fortuna);
        }
    }
}


} // namespace fortuna_source_file
