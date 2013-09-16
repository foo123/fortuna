#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <tuple>

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "server.hpp"


class Application
    : boost::noncopyable
{
public:
    struct Config
    {
        bool default_threads_num {true};
        unsigned custom_threads_num {0};

        constexpr
        Config()
        {}
    };

    struct AllConfig
        : public Server::AllConfig
    {
        Config application;
    };

private:
    const Config config;

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;
    boost::thread_group threads;

    Server server;

public:
    Application(AllConfig&& all_config = AllConfig());

    void run();
};

#endif // APPLICATION_HPP
