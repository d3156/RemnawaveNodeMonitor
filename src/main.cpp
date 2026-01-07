#include "checkNodes.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

std::chrono::seconds interval = std::chrono::seconds(1);

void tick(const boost::system::error_code & /*e*/, boost::asio::steady_timer &timer, CheckNodesData &data)
{
    checkNodes(data);
    timer.expires_at(timer.expires_at() + interval);
    timer.async_wait(std::bind(tick, std::placeholders::_1, std::ref(timer), std::ref(data)));
}

int main()
{
    std::string host(getenv("REMNAWAVE_HOST") ? getenv("REMNAWAVE_HOST") : "");
    std::string token(getenv("REMNAWAVE_TOKEN") ? getenv("REMNAWAVE_TOKEN") : "");
    std::string tokenTg(getenv("TELEGRAM_TOKEN") ? getenv("TELEGRAM_TOKEN") : "");
    std::string chat_idTG(getenv("TELEGRAM_CHAT_ID") ? getenv("TELEGRAM_CHAT_ID") : "");
    std::string cookie(getenv("REMNAWAVE_COOKIE") ? getenv("REMNAWAVE_COOKIE") : "");
    interval =
        std::chrono::seconds(atol(getenv("REMNAWAVE_INTERVAL") ? getenv("REMNAWAVE_INTERVAL") : "1")); // 1 second

    std::cout << "*************** RUN service ******************" << std::endl;
    std::cout << "host: " << host << std::endl;
    std::cout << "token: " << token << std::endl;
    std::cout << "cookie: " << cookie << std::endl;
    std::cout << "interval: " << interval.count() << std::endl;
    std::cout << "tokenTg: " << tokenTg << std::endl;
    std::cout << "chat_idTG: " << chat_idTG << std::endl;    
    std::cout << "*************** RUN service ******************" << std::endl;

    boost::asio::io_context io;
    auto guard = boost::asio::make_work_guard(io);
    boost::asio::steady_timer timer(io, interval);

    HttpClient clientRemnawave(io, host, cookie, token);
    HttpClient telegramClient(io, "api.telegram.org");
    CheckNodesData data{clientRemnawave, telegramClient, tokenTg, chat_idTG};

    timer.async_wait(std::bind(tick, std::placeholders::_1, std::ref(timer), std::ref(data)));

    io.run();
    return 0;
}