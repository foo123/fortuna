#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>

#include "server.hpp"


class Application
    : boost::noncopyable
{
private:
    boost::asio::io_service io_service;
    boost::asio::signal_set signals;
    boost::thread_group threads;

    Server server;

public:
    Application(int argc, char* argv[], Server::Config&& default_config = Server::Config());

    void run();
};

#endif // APPLICATION_HPP
