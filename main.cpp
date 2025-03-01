#include <iostream>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include "http/HttpClient.h"
#include "order-managment/OrderManager.h"
#include <nlohmann/json.hpp>
#include <iomanip>
#include "order-managment/OrderQueries.h"
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/asio/ssl/context.hpp>
#include "market-data/MarketDataStreamer.h"

using namespace std;
using json = nlohmann::json;

MarketDataStreamer marketDataStreamer;
string ws_connection_url = "wss://test.deribit.com/ws/api/v2";


void displayOptions()
{
    vector<string> actions = {
        "Place Order",
        "Get Open Orders",
        "Cancel Order",
        "Modify Order",
        "Get Order Book",
        "View Current Positions",
        "Subscribe to symbols",
        "Stream subscribed symbols"};

    cout << "\nPlease select an action:\n";
    for (size_t i = 0; i < actions.size(); ++i)
    {
        cout << (i + 1) << ". " << actions[i] << "\n";
    }
}

string getAccessToken(const char *clientId, const char *clientSecret)
{
    HttpClient httpClient;
    string url = "https://test.deribit.com/api/v2/public/auth";

    string jsonPayload = "{\"jsonrpc\": \"2.0\", "
                         "\"id\": 1, "
                         "\"method\": \"public/auth\", "
                         "\"params\": {"
                         "\"grant_type\": \"client_credentials\", "
                         "\"client_id\": \"" +
                         string(clientId) + "\", "
                                            "\"client_secret\": \"" +
                         string(clientSecret) + "\""
                                                "}}";

    vector<string> headers = {"Content-Type: application/json"};
    string response = httpClient.sendRequest (url, jsonPayload, headers);
    if (response.empty())
    {
        cerr << "Error retrieving access token." << endl;
        return "";
    }
    try
    {
        auto j = json::parse(response);
        if (j.contains("result") && j["result"].contains("access_token"))
        {
            return j["result"]["access_token"];
        }
        else
        {
            cerr << "Error retrieving access token." << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "JSON parsing error: " << e.what() << "\n";
        return "";
    }
    return "";
}

void welcomeMessage()
{
    cout << string(45, '-') << "\n";
    cout << "| Welcome to Deribit API client application |\n";
    cout << string(45, '-') << "\n";
}

void subscribeSymbols(const string &access_token)
{
    string symbol;
    cout << "Enter symbol to subscribe: ";
    cin >> symbol;
    marketDataStreamer.subscribe_orderbook(symbol);
}

int main()
{
    const char *clientId = getenv("CLIENT_ID");
    const char *clientSecret = getenv("CLIENT_SECRET");

    if (!clientId || !clientSecret)
    {
        cout << "set CLIENT_ID and CLIENT_SECRET environment variables\n";
        return 1;
    }
    marketDataStreamer.connect(ws_connection_url);

    int choice = 0;
    const string accessToken = getAccessToken(clientId, clientSecret);
    if (accessToken.empty())
    {
        cerr << "Error retrieving access token. Exiting...\n";
        return 1;
    }
    welcomeMessage();
    while (choice != 8)
    {
        displayOptions();
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            createOrder(accessToken);
            break;
        case 2:
            getOpenOrders(accessToken);
            break;
        case 3:
            cencelOrder(accessToken);
            break;
        case 4:
            modifyOrder(accessToken);
            break;
        case 5:
            getOrderBook(accessToken);
            break;
        case 6:
            viewCurrentPositions(accessToken);
            break;
        case 7:
            subscribeSymbols(accessToken);
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}