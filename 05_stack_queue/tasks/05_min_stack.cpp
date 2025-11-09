#include <algorithm>
#include <iostream>
#include <stack>

#define P(x) std::cout << "Log: " << x << std::endl;

class MinStack {
private:

public:
    MinStack() {}
    
    void push(int val) {
        // TODO
    }
    
    bool empty() {
        // TODO
    }

    void pop() {
        // TODO
    }
    
    int top() {
        // TODO
    }
    
    int getMin() {
        // TODO
    }
};


int main() {
    MinStack ms;

    ms.push(100);
    ms.push(200);
    ms.push(20);
    ms.push(3);
    ms.push(30);

    P(ms.top())
    P(ms.getMin())


    while(!ms.empty()) {
        ms.pop();
        P("----------")
        P(ms.top()) 
        P(ms.getMin())
    }


    return 0;
}