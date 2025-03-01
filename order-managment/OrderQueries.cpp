#include "OrderQueries.h"
#include "../http/HttpClient.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void getOpenOrders(const string &access_token)
{
    string url = "https://test.deribit.com/api/v2/private/get_open_orders";
    string jsonPayload = "{\"jsonrpc\": \"2.0\", "
                         "\"id\": 1, "
                         "\"method\": \"private/get_open_orders\", "
                         "\"params\": {"
                         "}}";

    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    HttpClient httpClient;
    string response = httpClient.sendRequest (url, jsonPayload, headers);
    try
    {
        auto jsonResponse = json::parse(response);
        if (jsonResponse.contains("result")){
            cout << "Open Orders: \n";
            cout << string(40, '-') << endl;
            for (auto &order : jsonResponse["result"]){
                cout << "Order ID: " << order["order_id"] << endl;
                cout << "Instrument Name: " << order["instrument_name"] << endl;
                cout << "Direction: " << order["direction"] << endl;
                cout << "Amount: " << order["amount"] << endl;
                cout << string(40, '-') << endl;

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

void getOrderBook(const string &access_token)
{
    string instrumentName;
    int depth;
    cout << "Enter instrument name: ";
    cin >> instrumentName;
    cout << "Enter depth: ";
    cin >> depth;

    string payload = "{\"jsonrpc\": \"2.0\", "
                     "\"id\": 1, "
                     "\"method\": \"public/get_order_book\", "
                     "\"params\": {"
                     "\"instrument_name\": \"" +
                     instrumentName + "\", "
                                      "\"depth\": " +
                     to_string(depth) +
                     "}}";
    
    string url = "https://test.deribit.com/api/v2/public/get_order_book";

    HttpClient httpClient;
    
    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    string response = httpClient.sendRequest (url, payload, headers);
    cout << response << endl;
    try
    {
        auto jsonResponse = json::parse(response);
        
        if (jsonResponse.contains("result"))
        {
            cout << "Order Book: \n";
            cout << string(65, '-') << endl;
            cout << "Instrument Name: " << jsonResponse["result"]["instrument_name"] << endl;
            cout << "Timestamp: " << jsonResponse["result"]["timestamp"] << endl;
            cout << "Bids: \n";
            cout << string(65, '-') << endl;

            for (auto &bid : jsonResponse["result"]["bids"])
            {
                cout << "Price: " << bid[0] << " Amount: " << bid[1] << endl;
            }
            cout << string(65, '-') << endl;

            cout << "Asks: \n";
            for (auto &ask : jsonResponse["result"]["asks"])
            {
                cout << "Price: " << ask[0] << " Amount: " << ask[1] << endl;
            }
            cout << string(65, '-') << endl;
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

void viewCurrentPositions(const string &access_token)
{
    string url = "https://test.deribit.com/api/v2/private/get_positions";
    string jsonPayload = "{\"jsonrpc\": \"2.0\", "
                         "\"id\": 1, "
                         "\"method\": \"private/get_positions\", "
                         "\"params\": {"
                         "}}";

    vector<string> headers = {"Authorization: Bearer " + access_token, "Content-Type: application/json"};
    HttpClient httpClient;
    string response = httpClient.sendRequest (url, jsonPayload, headers);
    try
    {
        auto jsonResponse = json::parse(response);
        if (jsonResponse.contains("result"))
        {
            cout << "jsonResponse: " << jsonResponse << endl;
            cout << "Current Positions: \n";
            cout << string(65, '-') << endl;
            for (auto &position : jsonResponse["result"]){
                cout << "Instrument Name: " << position["instrument_name"] << endl;
                cout << "Direction: " << position["direction"] << endl;
                cout << "Amount: " << position["amount"] << endl;
                cout << "Average Price: " << position["average_price"] << endl;
                cout << string(65, '-') << endl;
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