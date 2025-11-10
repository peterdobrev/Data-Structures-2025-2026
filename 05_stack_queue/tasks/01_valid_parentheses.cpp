#include <stack>
#include <iostream>
#include <string>

#define P(x) std::cout << "Log: " << x << std::endl;

bool isValid(std::string s) {
    std::stack<char> open;

    for (int i = 0; i < s.size(); i++) {
        char ch = s[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            open.push(ch);
        } else {
            if (open.empty()) {
                return false;
            }
            
            if (ch == ')' && open.top() != '(') {
                return false;
            }
            if (ch == ']' && open.top() != '[') {
                return false;
            }
            if (ch == '}' && open.top() != '{') {
                return false;
            }
            
            open.pop();
        }
    }

    return open.empty();
}

int main() {

    std::string input;
    while(true) {
        std::cin >> input;
        // input = "";
        // input = "()";
        // input = "({})";
        // input = "[[[)]]]";

        bool valid = isValid(input);
        if (valid) {
            P( input + " is valid")
        } else {
            P( input + " is Not valid")
        }
    }
}