#include "application.hpp"
#include "options.hpp"


Application::Application(int argc, char* argv[], Server::Config&& default_config)
    : io_service()
    , signals(io_service)
    , threads()
    , server(io_service, handle_options(argc, argv, std::move(default_config)))
{
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));
}

void Application::run()
{
    unsigned hardware_concurrency = boost::thread::hardware_concurrency();
    for (unsigned i = 0; i < hardware_concurrency; ++i)
        threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
    threads.join_all();
}
