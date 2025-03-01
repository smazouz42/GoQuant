#ifndef ORDERQUERIES_H
#define ORDERQUERIES_H

#include <string>

using namespace std;

void getOpenOrders(const string &access_token);
void getOrderBook(const string &access_token);
void viewCurrentPositions(const string &access_token);

#endif