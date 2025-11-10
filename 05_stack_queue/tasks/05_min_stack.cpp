#include <algorithm>
#include <iostream>
#include <stack>

#define P(x) std::cout << "Log: " << x << std::endl;

class MinStack {
private:
    std::stack<int> st;
    std::stack<int> min_st;
public:
    MinStack() {}
    
    void push(int val) {
        this->st.push(val);
        if(this->min_st.empty()) {
            this->min_st.push(val);
        } else {
            this->min_st.push(std::min(this->min_st.top(), val));
        }
    }
    
    bool empty() {
        return st.empty();
    }

    void pop() {
        this->st.pop();
        this->min_st.pop();
    }
    
    int top() {
        return this->st.top();
    }
    
    int getMin() {
        return this->min_st.top();
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