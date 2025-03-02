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
#include "utils/Utils.h"


using namespace std;
using json = nlohmann::json;

MarketDataStreamer marketDataStreamer;
string ws_connection_url = "wss://test.deribit.com/ws/api/v2";

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