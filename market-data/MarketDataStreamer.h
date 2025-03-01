#ifndef MARKETDATASTREAMER_H
#define MARKETDATASTREAMER_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/asio/ssl/context.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <string>

using json = nlohmann::json;
using websocketpp::connection_hdl;

class MarketDataStreamer {
public:
    MarketDataStreamer();
    void connect(const std::string& uri);
    void subscribe_orderbook(const std::string& symbol);

private:
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    client ws_client;
    std::thread ws_thread;
    connection_hdl ws_hdl;

    void on_open(connection_hdl hdl);
    void on_message(connection_hdl hdl, client::message_ptr msg);
};

#endif