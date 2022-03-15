#ifndef DHT_hpp
#define DHT_hpp

#include <stdio.h>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <openssl/sha.h>
#include <stdlib.h>

using namespace std;

class DHT {
private:
    // [hash key : physical IP]
    map<uint32_t, string> serverNodes; // ordered map to mimic circular ring for DHT
    set<string> physicalNode;  // simulated nodes to represent real machine nodes
    int vNum; // virtual Node Num
    int pNum;
public:
    DHT(int virtual_num, int physical_num);
    ~DHT() {
        serverNodes.clear();
    };
    static uint32_t Hash(string key);
    void Initialize();
    string PickStoreServer(const string& key);
    void StatisticPerf(int left, int right);
    string generateIP();
    static uint32_t hash(string key);
};

#endif /* DHT_hpp */