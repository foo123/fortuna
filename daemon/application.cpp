#include "application.hpp"

Application::Application(Config&& default_config = Config())
    : config(std::move(default_config))
    , accumulator(std::move(config.fortuna))
    , io_service()
    , signals(io_service)
    , server(io_service, accumulator, std::move(config.server))
    , threads()
{
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));
}

void Application::run()
{
    for (std::size_t i = 0; i < config.threads_count; ++i)
        threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
    threads.join_all();
}
