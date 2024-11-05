#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <climits>

using namespace std;

float scanNum(char ch) {
    return static_cast<float>(ch - '0');  // Convert char to float
}

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

bool isOperand(char ch) {
    return (ch >= '0' && ch <= '9');
}

float operation(float a, float b, char op) {
    switch (op) {
        case '+': return b + a; // Order matters for operand operations
        case '-': return b - a;
        case '*': return b * a;
        case '/': return b / a;
        case '^': return pow(b, a);
        default: return INT_MIN;  // Invalid operation
    }
}

float postfixEval(const string &postfix) {
    stack<float> stk;
    for (char ch : postfix) {
        if (isOperator(ch)) {
            float a = stk.top(); stk.pop();
            float b = stk.top(); stk.pop();
            stk.push(operation(a, b, ch));
        } else if (isOperand(ch)) {
            stk.push(scanNum(ch));
        }
    }
    return stk.top();
}

float evaluatePrefix(const string &prefix) {
    stack<float> stk;
    for (int i = prefix.length() - 1; i >= 0; i--) {
        char ch = prefix[i];
        if (isOperator(ch)) {
            float a = stk.top(); stk.pop();
            float b = stk.top(); stk.pop();
            stk.push(operation(a, b, ch));
        } else if (isOperand(ch)) {
            stk.push(scanNum(ch));
        }
    }
    return stk.top();
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

float evaluateInfix(const string &infix) {
    stack<float> values;
    stack<char> ops;

    for (size_t i = 0; i < infix.length(); i++) {
        char ch = infix[i];

        if (isOperand(ch)) {
            float val = 0;
            while (i < infix.length() && isOperand(infix[i])) {
                val = (val * 10) + (infix[i] - '0');
                i++;
            }
            i--;  // Decrement i to offset the for loop increment
            values.push(val);
        } else if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            while (!ops.empty() && ops.top() != '(') {
                float b = values.top(); values.pop();
                float a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(operation(a, b, op));
            }
            ops.pop();  // Pop the '('
        } else if (isOperator(ch)) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                float b = values.top(); values.pop();
                float a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(operation(a, b, op));
            }
            ops.push(ch);
        }
    }

    while (!ops.empty()) {
        float b = values.top(); values.pop();
        float a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(operation(a, b, op));
    }

    return values.top();
}

string getExpressionType(const string &exp) {
    if (isOperator(exp[0])) return "Prefix";
    if (isOperator(exp[exp.length() - 1])) return "Postfix";
    return "Infix";
}

bool isValidExpression(const string &exp) {
    int balance = 0;
    for (char ch : exp) {
        if (ch == '(') balance++;
        else if (ch == ')') balance--;
        if (balance < 0) return false;
    }
    return balance == 0;
}

int main() {
    string expression;
    cout << "Enter an expression (infix, prefix, or postfix): ";
    getline(cin, expression);

    if (!isValidExpression(expression)) {
        cout << "Invalid expression!" << endl;
        return 0;
    }

    string type = getExpressionType(expression);
    cout << "Expression Type: " << type << endl;

    float result;
    if (type == "Prefix") {
        result = evaluatePrefix(expression);
    } else if (type == "Postfix") {
        result = postfixEval(expression);
    } else if (type == "Infix") {
        result = evaluateInfix(expression);
    } else {
        cout << "Unknown expression type!" << endl;
        return 0;
    }

    cout << "Evaluated Result: " << result << endl;

    return 0;
}