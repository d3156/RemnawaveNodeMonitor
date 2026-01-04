#pragma once
#include "HttpClient.hpp"

struct CheckNodesData{
    HttpClient &clientRemnawave;
    HttpClient& telegramClient;
    std::string telegramToken;
    std::string telegramChatId;
};

void checkNodes(CheckNodesData &data);