/*
Copyright 2013 Adam Mizerski <adam@mizerski.pl>

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

#include "seed_file_manager.hpp"

#include <fstream>
#include <functional>

#include <cryptopp/secblock.h>

#include "accumulator.hpp"
#include "fortuna_exception.hpp"


namespace fortuna {


SeedFileManager::SeedFileManager(Config _config, Accumulator& _accumulator)
    : config{std::move(_config)}
    , accumulator(_accumulator)
{
    update_seed_file();
    repeating_task.start(config.write_interval, std::bind(&SeedFileManager::write_seed_file, this));
}

void SeedFileManager::write_seed_file()
{
    const std::size_t seed_file_length_in_blocks = Accumulator::bytes_to_blocks(config.seed_file_length);
    CryptoPP::SecByteBlock buffer{seed_file_length_in_blocks * Accumulator::output_block_length};
    accumulator.get_random_data(buffer.BytePtr(), seed_file_length_in_blocks);

    // TODO: this is not atomic
    std::ofstream filestream{config.seed_file_path, std::ios::binary|std::ios::trunc};
    filestream.write(reinterpret_cast<const char*>(buffer.BytePtr()), config.seed_file_length);
}

void SeedFileManager::update_seed_file()
{
    using namespace std::placeholders;

    CryptoPP::SecByteBlock buffer{config.seed_file_length};

    {
        std::ifstream filestream{config.seed_file_path, std::ios::binary};
        if (!filestream)
            throw FortunaException::seed_file_error("could not open seed file");

        filestream.read(reinterpret_cast<char*>(buffer.BytePtr()), config.seed_file_length);
        if (static_cast<std::size_t>(filestream.gcount()) != config.seed_file_length)
            throw FortunaException::seed_file_error("could not read seed");
    }

    accumulator.monitored_generator.exec_rw(std::bind(&Generator::reseed, _1, buffer, buffer.SizeInBytes()));
    write_seed_file();
}


} // namespace fortuna
