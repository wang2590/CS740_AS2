#include "DHT.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <math.h>

#define IPV4_SIZE 32
#define M 32-1

using namespace std;

/*
 * DHT constructor, initialize all the nodes 
 */
DHT::DHT(int virtual_num, int physical_num){
    vNum = virtual_num;
    pNum = physical_num;
    for(int i =0; i < pNum;i++){
        physicalNode.insert(generateIP());
    }
    Initialize();
};

/*
 * Helper Hash SHA1 from opnessl 
 */
uint32_t DHT::hash(string key){
    unsigned char buffer[key.size()];
    copy(key.begin(), key.end(), buffer);
    unsigned char resultHash [key.size()];    

    SHA1(buffer, sizeof(buffer), resultHash);
    string temp = "";
    char convert_Hash [key.size()];
    for(int i = 0; i < key.size()/2; i++){
        // convert to decimal form
        sprintf(convert_Hash, "%d", resultHash[i]);
        temp += convert_Hash;
    }
    uint32_t mod = pow(2, M);
    long long int res = stoll(temp) % mod;
    return (uint32_t)res;
}

// https://create.stephan-brumme.com/fnv-hash/
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
/*
 * FNV hash function with 32bits reference above
 */
uint32_t DHT::Hash(string key) {
    const int p = 16777619;
    uint32_t hash = 2166136261;
    for (int idx = 0; idx < key.size(); ++idx) {
        hash = (hash ^ key[idx]) * p;
    }
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    if (hash < 0) {
        hash = -hash;
    }
    return hash;
}

/*
 * Sets node IP address to be an IPV4
 */
string DHT::generateIP() {
    string res;
    for(int i = 0; i < IPV4_SIZE/8; i++){
        if(i != 2)
            res += to_string(rand() % 255 + 1) + ".";
        else
            res += to_string(rand() % 9 + 1)+".";
    }
    res.pop_back();
    return res;
}

/*
 * hash each physical nodes' ip insert to map structure 
 */
void DHT::Initialize()
{
    for(auto& ip : physicalNode) {
        for(int j = 0; j < vNum; ++j) {
            stringstream nodeKey;
            nodeKey << ip << "#" << j;
            uint32_t partition = Hash(nodeKey.str());
            serverNodes.insert({partition, ip});
        }
    }
}

/*
 * hash data points, store in a chosen virtual node
 */
string DHT::PickStoreServer(const string& key)
{
    uint32_t partition = Hash(key);
    // iterator pointing to the first element in the container
    // whose key is not considered to go before k 
    // (i.e., either it is equivalent or goes after).
    auto it = serverNodes.lower_bound(partition);
    if(!serverNodes.empty() && it == serverNodes.end()) { // loop around pass 0th node
        return serverNodes.begin()->second;
    }
    return it->second;
}

/*
 * print outs for load balance stats
 */
void DHT::StatisticPerf(int start, int end) {
    map<string, int> collectStat; // [physical Node : data store hit]
    for (int i = start; i <= end; i++) {
        string nodeIp = PickStoreServer(to_string(i));
        collectStat[nodeIp]++;
    }
    cout << "\n==== "<<vNum<<" virtual Nodes | "<<end-start<<" data points"<<" ====" << '\n';
    int i=1;
    for (auto& p : collectStat) {
        cout <<"#"<<i++<< " IP: " << p.first << " hit_rate: " << (p.second / (double)(end - start + 1)) * 100.0 << "%" << endl;
    }
}


