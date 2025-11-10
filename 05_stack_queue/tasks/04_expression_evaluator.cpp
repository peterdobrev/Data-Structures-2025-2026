#include <stack>
#include <iostream>
#include <string>
#include <cmath> // For NAN

// Get precedence of an operator
int getPrecedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0; // For parentheses
}

float evaluateOp(float lhs, float rhs, char op) {
    switch (op) {
        case '-': return lhs - rhs;
        case '+': return lhs + rhs;
        case '/': 
            if (rhs == 0.0f) {
                // Simplified error handling
                std::cerr << "Error: Division by zero.\n";
                return NAN; 
            }
            return lhs / rhs;
        case '*': return lhs * rhs;
        default: return NAN;
    }
}

// Helper function to apply an operation when precedence dictates
void applyOperation(std::stack<float>& operands, std::stack<char>& operators) {
    if (operands.size() < 2) {
        // Handle error: not enough operands
        std::cerr << "Error: Missing operand in expression.\n";
        return;
    }
    
    // 1. Pop the operator
    char op = operators.top();
    operators.pop();

    // 2. Pop operands (RHS first, then LHS)
    float rhs = operands.top();
    operands.pop();
    float lhs = operands.top(); 
    operands.pop();

    // 3. Evaluate and push result
    float res = evaluateOp(lhs, rhs, op);
    operands.push(res);
}

float evaluate(std::string s) {
    std::stack<float> operands;
    std::stack<char> operators;

    char op;
    float num;

    for( char ch : s) {
        // Skip whitespace
        if (ch == ' ') continue;

        if (ch >= '0' && ch <= '9') {
            operands.push((float)(ch - '0'));
            continue;
        }
        if (ch == '(') {
            operators.push('(');
            continue;
        }

        if (ch == ')') {
            while(!operators.empty() && operators.top() != '(') {
                applyOperation(operands, operators);
            }
            
            // Check for mismatched parentheses
            if (operators.empty()) {
                std::cerr << "Error: Mismatched parentheses (missing '(').\n";
                return NAN;
            }

            // Discard the opening parenthesis
            operators.pop();
            continue;
        }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            char current_op = ch;
            
            // While the operator stack is NOT empty AND 
            // the top is NOT a left parenthesis AND 
            // the current operator has lower or equal precedence than the top operator
            while(!operators.empty() && operators.top() != '(' && 
                getPrecedence(operators.top()) >= getPrecedence(current_op)) 
            {
                applyOperation(operands, operators);
            }

            // Push the current operator onto the stack
            operators.push(current_op);
            continue;
        }

        // Case 5: Invalid character
        std::cerr << "Error: Invalid character '" << ch << "' in expression.\n";
        return NAN;
    }

    while(!operators.empty()) {
        applyOperation(operands, operators);
    }

    // The final result should be the only item left on the operand stack
    if (operands.size() == 1) {
        return operands.top();
    } else if (operands.empty()) {
        // Empty or invalid expression
        return 0.0f; 
    } else {
        std::cerr << "Error: Invalid expression structure (too many numbers).\n";
        return NAN;
    }
}

int main() {
    std::cout << "Enter expression (single-digit numbers only, e.g., 1+4*2):\n";
    std::string input;
    
    // Interactive loop
    while(std::getline(std::cin, input)) {
        if (input.empty()) continue;
        if (input == "exit") break;
        float result = evaluate(input);
        std::cout << "Result: " << result << "\n";
    }

    return 0;
}