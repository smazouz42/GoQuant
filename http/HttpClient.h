#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>

using namespace std;

class HttpClient {
public:
    HttpClient();
    ~HttpClient();
    string sendHttpPost(const string &url, const string &jsonPayload, const vector<string> &headers);

private:
    CURL *curl;
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif
