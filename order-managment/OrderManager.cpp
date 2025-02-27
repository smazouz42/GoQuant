#include <iostream>
#include "OrderManager.h"
#include "../http/HttpClient.h"
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

void createOrder(const string &access_token)
{
    string symbol, side;
    double price;
    int quantity;
    cout << "\nEnter order details:\n";
    cout << "Symbol: ";
    cin >> symbol;
    cout << "Side (Buy/Sell): ";
    cin >> side;
    cout << "Price: ";
    cin >> price;
    cout << "Quantity: ";
    cin >> quantity;

    transform(side.begin(), side.end(), side.begin(), ::tolower);
    string methodName = (side == "buy") ? "private/buy" : "private/sell";
    string orderRequest = "{\"jsonrpc\": \"2.0\", "
                          "\"id\": 1, "
                          "\"method\": \"" +
                          methodName + "\", "
                                       "\"params\": {"
                                       "\"instrument_name\": \"" +
                          symbol + "\", "
                                   "\"amount\": " +
                          to_string(quantity) + ", "
                                                "\"price\": " +
                          to_string(price) + ", "
                                             "\"type\": \"limit\""
                                             "}}";

    string url = "https://test.deribit.com/api/v2/" + methodName;
    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    HttpClient httpClient;
    string response = httpClient.sendHttpPost(url, orderRequest, headers);
    try
    {
        auto jsonResponse = json::parse(response);
        if (jsonResponse.contains("result") && jsonResponse["result"].contains("order"))
        {

            auto order = jsonResponse["result"]["order"];
            if (order.contains("order_id"))
            {
                cout << "Order ID: " << order["order_id"] << endl;
            }
            else
            {
                cout << "Order ID not found in the order details.\n";
            }
        }
        else
        {
            cerr << "Error : faild with error msg :" << jsonResponse["error"]["message"] << "\n";
        }
    }
    catch (const std::exception &e)
    {
        cerr << "JSON parsing error: " << e.what() << "\n";
    }
}

void cencelOrder(const string &access_token)
{
    string orderId;
    cout << "Enter Order ID: ";
    cin >> orderId;

    string url = "https://test.deribit.com/api/v2/private/cancel";
    string jsonPayload = "{\"jsonrpc\": \"2.0\", "
                         "\"id\": 1, "
                         "\"method\": \"private/cancel\", "
                         "\"params\": {"
                         "\"order_id\": \"" +
                         orderId + "\""
                                   "}}";

    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    HttpClient httpClient;
    string response = httpClient.sendHttpPost(url, jsonPayload, headers);

    try
    {
        auto jsonResponse = json::parse(response);
        if (jsonResponse.contains("result") && jsonResponse["result"].contains("instrument_name"))
        {
            cout << "Order Named: " << jsonResponse["result"]["instrument_name"] << " has been cancelled.\n";
        }
        else
        {
            cerr << "Error : faild with error msg :" << jsonResponse["error"]["message"] << "\n";
        }
    }
    catch (const std::exception &e)
    {
        cerr << "JSON parsing error: " << e.what() << "\n";
    }
}

void modifyOrder(const string &access_token)
{
    string orderId;
    double price;
    int quantity;
    cout << "Enter Order ID: ";
    cin >> orderId;
    cout << "Enter new price: ";
    cin >> price;
    cout << "Enter new quantity: ";
    cin >> quantity;

    string url = "https://test.deribit.com/api/v2/private/edit";
    string jsonPayload = "{\"jsonrpc\": \"2.0\", "
                         "\"id\": 1, "
                         "\"method\": \"private/edit\", "
                         "\"params\": {"
                         "\"order_id\": \"" +
                         orderId + "\", "
                                   "\"amount\": " +
                         to_string(quantity) + ", "
                                               "\"price\": " +
                         to_string(price) + ", "
                                            "\"post_only\": true"
                                            "}}";

    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    HttpClient httpClient;
    string response = httpClient.sendHttpPost(url, jsonPayload, headers);

    try
    {
        auto jsonResponse = json::parse(response);
        if (jsonResponse.contains("result") && jsonResponse["result"].contains("order"))
        {
            auto order = jsonResponse["result"]["order"];
            if (order.contains("order_id"))
            {
                cout << "Order ID: " << order["order_id"] << " has been modified.\n";
            }
            else
            {
                cout << "Order ID not found in the order details.\n";
            }
        }
        else
        {
            cerr << "Error : faild with error msg :" << jsonResponse["error"]["message"] << "\n";
        }
    }
    catch (const std::exception &e)
    {
        cerr << "JSON parsing error: " << e.what() << "\n";
    }
}