#include "Utils.h"
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "../http/HttpClient.h"

using namespace std;
using json = nlohmann::json;

void displayOptions()
{
    vector<string> actions = {
        "Place Order",
        "Get Open Orders",
        "Cancel Order",
        "Modify Order",
        "Get Order Book",
        "View Current Positions",
        "Subscribe to symbols"};

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
    string response = httpClient.sendRequest(url, jsonPayload, headers);
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