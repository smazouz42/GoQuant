#include "HttpClient.h"

HttpClient::HttpClient() {
    curl = curl_easy_init();
}

HttpClient::~HttpClient() {
    curl_easy_cleanup(curl);
}

string HttpClient::sendRequest (const string &url, const string &jsonPayload, const vector<string> &headers) {
    string response;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());

        struct curl_slist *headerList = NULL;
        for(const string &header : headers) {
            headerList = curl_slist_append(headerList, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "Failed to send HTTP POST request: " << curl_easy_strerror(res) << endl;
        }
        curl_slist_free_all(headerList);
    }
    else
    {
        cerr << "Failed to initialize curl in sendRequest .\n";
    }
    return response;
}

size_t HttpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
