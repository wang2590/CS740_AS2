#include "DHT.hpp"
#include <time.h>

int main() {
    srand(time(NULL));
    // each physical node has 1 vitural nodes
    DHT ch(1, 5);
    ch.StatisticPerf(50, 1000); 
    // each physical node has 50 vitural nodes
    DHT ch2(50,5);
    ch2.StatisticPerf(50, 1000);
    // each physical node has 100 vitural nodes
    DHT ch3(100,5);
    ch3.StatisticPerf(50, 1000);
    return 0;
}

// build & run CMD: g++ DHT.cpp main.cpp -L/usr/lib -lssl -lcrypto && ./a.out