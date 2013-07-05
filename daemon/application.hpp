#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include <fortuna/accumulator.hpp>

#include "server.hpp"


class Application
    : boost::noncopyable
{
public:
    struct Config
    {
        std::size_t threads_count {4};
    };

private:
    Config config;

    fortuna::Accumulator accumulator;

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;

    Server server;

    boost::thread_group threads;

public:
    Application(Config&& default_config = Config());

    void run();
};

#endif // APPLICATION_HPP
