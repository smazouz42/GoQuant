#include "MarketDataStreamer.h"

MarketDataStreamer::MarketDataStreamer() {
    ws_client.init_asio();
    ws_client.set_tls_init_handler([](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(
            boost::asio::ssl::context::tlsv12);
    });

    ws_client.set_open_handler(bind(&MarketDataStreamer::on_open, this, std::placeholders::_1));
    ws_client.set_message_handler(bind(&MarketDataStreamer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void MarketDataStreamer::connect(const std::string& uri) {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = ws_client.get_connection(uri, ec);
    if (ec) {
        std::cerr << "Connection error: " << ec.message() << std::endl;
        return;
    }
    ws_client.connect(con);
    ws_thread = std::thread([this]() { ws_client.run(); });
}

void MarketDataStreamer::subscribe_orderbook(const std::string& symbol) {
    json request = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "public/subscribe"},
        {"params", {{"channels", {"book." + symbol + ".100ms"}}}}
    };
    websocketpp::lib::error_code ec;
    ws_client.send(ws_hdl, request.dump(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cerr << "Send error: " << ec.message() << std::endl;
    } else {
        std::cout << "Subscribed to " << symbol << " orderbook" << std::endl;
    }
}

void MarketDataStreamer::on_open(connection_hdl hdl) {
    ws_hdl = hdl;
    std::cout << "Connected to Deribit WebSocket!" << std::endl;
}

void MarketDataStreamer::on_message(connection_hdl hdl, client::message_ptr msg) {
    try {
        auto j = json::parse(msg->get_payload());
        if (j.contains("params") && j["params"].contains("data")) {
            auto data = j["params"]["data"];
            if (data.contains("asks") || data.contains("bids")) {
                std::cout << "Order Book For " << data["instrument_name"] << " at " << data["timestamp"] << std::endl << std::endl;

                if (data.contains("asks")) {
                    std::cout << std::string(40, '-') << std::endl;
                    std::cout << std::string(10, ' ') << "\nAsks:\n";
                    std::cout << "Price" << std::string(15, ' ') << "Amount" << std::string(14, ' ') << std::endl;
                    std::cout << std::string(40, '-') << std::endl;
                    int k = 0;
                    for (const auto& ask : data["asks"]) {
                        std::cout << ask[1] << std::string(20 - ask[1].size(), ' ') << ask[2] << std::string(20 - ask[2].size(), ' ') << std::endl;
                        if (++k == 10) break;
                    }
                }
                if (data.contains("bids")) {
                    std::cout << std::string(40, '-') << std::endl;
                    std::cout << std::string(10, ' ') << "\nBids:\n";
                    std::cout << "Price" << std::string(15, ' ') << "Amount" << std::string(14, ' ') << std::endl;
                    std::cout << std::string(40, '-') << std::endl;
                    int k = 0;
                    for (const auto& bid : data["bids"]) {
                        std::cout << bid[1] << std::string(20 - bid[1].size(), ' ') << bid[2] << std::string(20 - bid[2].size(), ' ') << std::endl;
                        if (++k == 10) break;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}