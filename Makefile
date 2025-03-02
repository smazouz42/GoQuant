CXX = g++

CXXFLAGS = -std=c++11 -Wall

INCLUDES = -I./http -I./order-managment -I./websocketpp -I./market-data  -I./utils

LIBS = -lcurl -lssl -lcrypto

SRCS = main.cpp http/HttpClient.cpp order-managment/OrderManager.cpp order-managment/OrderQueries.cpp market-data/MarketDataStreamer.cpp utils/Utils.cpp
OBJS = $(SRCS:.cpp=.o)

EXEC = GoQuant

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(EXEC) $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC)

.PHONY: all clean