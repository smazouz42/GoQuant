#include <iostream>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include "http/HttpClient.h"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void displayOptions()
{
    vector<string> actions = {
        "Place Order",
        "Cancel Order",
        "Modify Order",
        "Get Order Book",
        "View Current Positions",
        "Subscribe to symbols",
        "Stream subscribed symbols"};

    cout << "\nPlease select an action:\n";
    for (int i = 0; i < actions.size(); ++i)
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
    string response = httpClient.sendHttpPost(url, jsonPayload, headers);
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

int main()
{

    const char *clientId = getenv("CLIENT_ID");
    const char *clientSecret = getenv("CLIENT_SECRET");

    if (!clientId || !clientSecret)
    {
        cout << "Please set CLIENT_ID and CLIENT_SECRET environment variables\n";
        return 1;
    }

    int choice = 0;
    string accessToken = getAccessToken(clientId, clientSecret);
    if (accessToken.empty())
    {
        cerr << "Error retrieving access token. Exiting...\n";
        return 1;
    }
    displayOptions();
    while (choice != 8)
    {

        cout << "\nPlease Enter your choice: \n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Place Order\n";
            break;
        case 2:
            cout << "Cancel Order\n";
            break;
        case 3:
            cout << "Modify Order\n";
            break;
        case 4:
            cout << "Get Order Book\n";
            break;
        case 5:
            cout << "View Current Positions\n";
            break;
        case 6:
            cout << "Subscribe to symbols\n";
            break;
        case 7:
            cout << "Stream subscribed symbols\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}