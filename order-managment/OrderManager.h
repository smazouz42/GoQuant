#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H


using namespace std;
#include <string>

void createOrder(const string &access_token);
void modifyOrder(const string &access_token);
void cencelOrder(const string &access_token);

#endif