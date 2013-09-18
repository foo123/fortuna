#include "application.hpp"


Application::Application(Application::AllConfig&& all_config)
    : config(std::move(all_config.application))
    , io_service()
    , signals(io_service)
    , threads()
    , server(io_service, std::move(static_cast<Server::AllConfig>(all_config)))
{
    signals.add(SIGINT);
    signals.add(SIGTERM);
    signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));
}

void Application::run()
{
    const unsigned threads_num =
        config.default_threads_num
            ? boost::thread::hardware_concurrency()
            : config.custom_threads_num
    ;
    for (unsigned i = 0; i < threads_num; ++i)
        threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
    threads.join_all();
}
