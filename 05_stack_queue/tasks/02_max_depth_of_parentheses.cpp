#include <stack>
#include <iostream>
#include <string>
#include <algorithm> // For std::max

#define P(x) std::cout << "Log: " << x << std::endl;

int getMaxParenthesesDepth(std::string s) {
    int max_depth = 0;
    std::stack<char> opened;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        
        if ( c == '(' || c == '[' || c == '{') {
            opened.push(c);
            max_depth = std::max(max_depth, (int)opened.size());

        } else if ( c == ')' || c == ']' || c == '}') {

            if (!opened.empty()) {
                opened.pop();
            }
        }

    }

    return max_depth;
}

int main() {

    std::string input;
    while(true) {
        std::cin >> input;
        // input = "";
        // input = "()";
        // input = "({})";
        // input = "[[[)]]]";

        int depth = getMaxParenthesesDepth(input);
        P("depth is: " + std::to_string(depth) )
    }
}