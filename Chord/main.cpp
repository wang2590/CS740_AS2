#include <iostream>
#include "src/Node.h"
#include <time.h>

using namespace std;

int main() {
    srand(time(NULL));

    Node j;
    j.join(nullptr);
    int num = 32; // 32 nodes to be joined
    Node joinNodes[num];
    for(int i = 0; i < num; i++){ 
        joinNodes[i].join(&j);
    }
    cout << "\ninitial node";
    j.print_finger_table();
    
    for(int i = 0; i < num; i++){ 
        cout << "\n#: "<<i <<" joined node";
        joinNodes[i].print_finger_table();
    }

    return 0;
}
