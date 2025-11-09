#include <stack>
#include <iostream>
#include <string>

#define P(x) std::cout << "Log: " << x << std::endl;

bool isValid(std::string s) {
    // TODO implement me
    return false;
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