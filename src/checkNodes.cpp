#include "checkNodes.hpp"
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/string_body.hpp>
#include <iostream>
#include <boost/json.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
namespace http = beast::http;

namespace json = boost::json;

using namespace boost::posix_time;
using namespace boost::gregorian;

std::string currentTime() { return to_simple_string(second_clock::universal_time() + hours(3)); }

void checkNodes(CheckNodesData &data)
{
    try {

        static int messageId = 0;
        http::request<http::string_body> req{http::verb::get, "/api/nodes", 11};
        http::response<http::dynamic_body> res = data.clientRemnawave.send(req);

        json::value root = json::parse(boost::beast::buffers_to_string(res.body().data()));

        const auto &response = root.at("response").as_array();

        std::string status = "Сосотяние нод:\n";

        for (const auto &node : response) {
            const auto &obj = node.as_object();

            std::string name        = json::value_to<std::string>(obj.at("name"));
            std::string uuid        = json::value_to<std::string>(obj.at("uuid"));
            std::string countryCode = json::value_to<std::string>(obj.at("countryCode"));
            bool isConnected        = obj.at("isConnected").as_bool();
            status += (isConnected ? "  🟢 " : "  🟡 ") + name + "\n";
        }

        status += "\nДата обновления: " + currentTime();

        std::cout << "[CheckNodes] " << status << std::endl;

        json::object body_json;
        body_json["chat_id"] = data.telegramChatId;
        body_json["text"]    = status;
        if (messageId != 0) {
            body_json["message_id"] = messageId;
            http::request<http::string_body> reqtg{http::verb::get, "/bot" + data.telegramToken + "/editMessageText",
                                                    11};
            reqtg.body() = json::serialize(body_json);
            data.telegramClient.send(reqtg);
        } else {
            http::request<http::string_body> reqtg{http::verb::get, "/bot" + data.telegramToken + "/sendMessage", 11};
            reqtg.body() = json::serialize(body_json);
            http::response<http::dynamic_body> res = data.telegramClient.send(reqtg);
            json::value root                   = json::parse(boost::beast::buffers_to_string(res.body().data()));
            const auto &obj                        = root.as_object();
            messageId                              = obj.at("result").as_object().at("message_id").as_int64();
        }
    } catch (const std::exception &e) {
        std::cerr << "[CheckNodes] Error: " << e.what() << std::endl;
    }
}
