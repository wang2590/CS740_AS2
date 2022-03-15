#include "Node.h"
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <set>

set<int> st;
using namespace std;

#define successor finger[0].node

Node::Node() {
    ip_address = generateIP();
    identifier = hash(ip_address);
    if(st.find(identifier) == st.end()){
        st.insert(identifier);
    }else{
        identifier+=2;
        int mod = pow(2,M);
        identifier%=mod;
        st.insert(identifier);
    }
    fillFTStart();
    primary = nullptr;
}

/*
 * Sets node IP address to be an IPV4
 */
string Node::generateIP() {
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
 * Helper Hash SHA1 from opnessl 
 */
int Node::hash(string key){
    unsigned char buffer[key.size()];
    copy(key.begin(), key.end(), buffer);
    unsigned char resultHash [key.size()];    

    SHA1(buffer, sizeof(buffer), resultHash);
    string temp = "";
    char convert_Hash [key.size()];
    for(int i = 0; i < key.size()/8; i++){
        // convert to decimal form
        sprintf(convert_Hash, "%d", resultHash[i]);
        temp += convert_Hash;
    }

    // int res = stoll(temp) % MOD;
    int mod = pow(2, M);
    int res = stoll(temp) % mod;
    return res;
}

/*
 * Return closest finger preceding id
 */
Node* Node::closest_preceding_finger(int id) {
    for(int i = M-1; i >= 0; i--){
        if(finger[i].node->identifier > identifier && finger[i].node->identifier < id){
            return finger[i].node;
        }
    }
    return this;
}

/*
 * Ask node n to find id's predecessor
 */
Node* Node::find_predecessor(int id) {
    Node* n_prime = primary;
    Node* prev;
    while(!(id > n_prime->identifier && id <= n_prime->successor->identifier)){
        n_prime = n_prime->closest_preceding_finger(id);
        if(prev == n_prime){
            return n_prime;
        }
        prev = n_prime;
    }
    return n_prime;
}

/*
 * Ask node n to find id's successor
 */
Node* Node::find_successor(int id) {
    Node *n_prime = find_predecessor(id);
    return n_prime->successor;
}

/*
 * Let node n join the network
 */
void Node::join(Node* n_prime){
    if(n_prime != nullptr) {
        primary = n_prime;
        init_finger_table(n_prime);
        update_others();
    }
    // node is first node;
    else {
        for(int i = 0; i < M; i++){
            finger[i].node = this;
        }
        predecessor = this;
        primary = this;
    }
}

/*
 * initialize finger table of local node;
 */
void Node::init_finger_table(Node *n_prime) {
    finger[0].node = n_prime->find_successor(finger[0].start);
    predecessor = successor->predecessor;
    successor->predecessor = this;
    successor_ptr = successor;
    ////
    update_succ_pred_FT();
    ////
    // cout<<"id: "<<identifier<<" successor: "<<successor_ptr->identifier<<" predecessor: "<<predecessor->identifier <<endl;
    
    for(int i = 0; i < M-1; i++){
        if((finger[i+1].start >= identifier) && (finger[i+1].start < finger[i].node->identifier)){
            finger[i+1].node = finger[i].node;
        }
        else {
            finger[i+1].node = n_prime->find_successor(finger[i+1].start);
            ////////////
            if(finger[i+1].start > finger[i+1].node->identifier && finger[i+1].start < identifier && finger[i+1].node->identifier < identifier){
                finger[i+1].node = this;
            }
            ////////////
            // cout <<"start: "<<finger[i+1].start <<", found succ:"<<finger[i+1].node->identifier <<endl;
        }
    }
}

/*
 * update predecessor & successor finger tables when n joins
 */
void Node::update_succ_pred_FT(){
    // update predecessor's table
    Node* p = predecessor;
    Node* s = successor_ptr;
    if(p->successor_ptr->identifier > pow(2,M))
        p->successor_ptr = p->predecessor;
    if(s->successor_ptr->identifier > pow(2,M))
        s->successor_ptr = s->predecessor;
    for(int i = 0; i < M; i++){
        // cout <<"I am:"<<p->identifier<<" P:"<<p->predecessor->identifier<<" S:"<<p->successor_ptr->identifier<<endl;
        if((p->finger[i].start <= p->successor_ptr->identifier && p->successor_ptr->identifier < p->finger[i].node->identifier)){
            p->finger[i].node = p->successor_ptr;
        }
        if(p->finger[i].start <= p->successor_ptr->identifier && p->finger[i].start > p->identifier){
            p->finger[i].node = p->successor_ptr;
        }
    }
    // update successor's table
    for(int i = 0; i < M; i++){
        if(s->finger[i].start <= s->predecessor->identifier && s->predecessor->identifier < s->finger[i].node->identifier){
            s->finger[i].node = s->predecessor;
        }
    } 
}

/*
 * update finger tables when n joins
 */
void Node::update_others() {   
    // in-between check
    for(int i = 0; i < M; i++){ 
        // find last node p whose ith finger might be node n
        Node* p = find_predecessor(identifier-pow(2, i));
        p->update_finger_tables(this, i);
    }
}

/*
 * update the finger table for the current node at pos i with Node s
 */
void Node::update_finger_tables(Node *s, int i) {
    // cout <<"s->identifier:" <<s->identifier<<" this->identifier:"<<identifier<<" finger["<<i<<"].node->identifier: "<<finger[i].node->identifier<<endl;
    if((s->identifier >= identifier) && (s->identifier < finger[i].node->identifier)){
        finger[i].node = s;
        Node* p = predecessor; // get first node preceding node n
        p->update_finger_tables(s, i);
    }
}

/*
 * returns n + 2^(k-1) mod 2^m
 */
int Node::start(int k){
    int power = pow(2, k-1);
    int modulo_val = pow(2, M);
    return (identifier + power) % modulo_val;
    // return (identifier + power) % MOD;
}

/*
 * fill node's finger table on initialize
 */
void Node::fillFTStart(){
    for(int i =0; i<M; i++){
        finger[i].start = start(i+1);
        if(i == M-1)
            finger[i].interval={finger[i].start, finger[0].start};
        else
            finger[i].interval={finger[i].start, start(i+2)};
    }
}

/*
 * print node finger table
 */
void Node::print_finger_table(){
    cout << "\n#### IP:"<<ip_address<<" | id:"<<identifier<<" ####"<<endl;
    for(int i =0; i< M; i++){
        cout << "#"<<i+1<<": "<<"Start:"<<finger[i].start<<" interval:["<<finger[i].interval.first<<","<<finger[i].interval.second<<")"<<" Succ_id: "<<finger[i].node->getIdentifier()<<endl;
    }
    cout <<"Successor: " <<successor->identifier<<" Predecessor: "<<predecessor->identifier<<endl;
}