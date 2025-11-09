#include <stack>
#include <iostream>
#include <string>

#define P(x) std::cout << "Log: " << x << std::endl;

int getMaxParenthesesDepth(std::string s) {
    // TODO implement me
    return 0;
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