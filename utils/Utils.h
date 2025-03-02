#ifndef UTILS_H
#define UTILS_H

#include <string>

void displayOptions();
std::string getAccessToken(const char *clientId, const char *clientSecret);
void welcomeMessage();

#endif