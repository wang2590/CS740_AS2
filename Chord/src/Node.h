#ifndef CHORD_NODE_H
#define CHORD_NODE_H

#include <string>
#include <utility>

#define M 7 // picked identifier/key bit length
#define IPV4_SIZE 32
#define MOD 300

using namespace std;

class Node {

struct Finger_T { // k: ith finger table entry
    int start; // n + 2^(k-1) mod 2^m
    pair<int,int> interval; // [finger[k].start, finger[k+1].start)
    Node* node; // first node >= n.finger[k].start    
};

private:
    string generateIP();
    string ip_address;
    int identifier;
    Finger_T finger[M];
    Node* predecessor;
    Node* successor_ptr;
    Node* primary;
    int start(int k);
    void fillFTStart();
public:
    Node();
    Node* find_successor(int id);
    Node* find_predecessor(int id);
    Node* closest_preceding_finger(int id);
    void join(Node* n_prime);
    void init_finger_table(Node* n_prime);
    void update_others();
    void update_finger_tables(Node* s, int i);
    void update_succ_pred_FT();
    int getIdentifier(){return identifier;};
    string getIP(){return ip_address;};
    int getPredecessor(){return predecessor->getIdentifier();};
    int getSuccessor(){return successor_ptr->getIdentifier();};
    Node* getPredecessorPtr(){return predecessor;};
    Node* getSuccessorPtr(){return successor_ptr;};
    void print_finger_table();
    int hash(string key);
};

#endif //CHORD_NODE_H