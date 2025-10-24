#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <fstream>
using namespace std;

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Function to perform arithmetic operations
int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;  // prevent division by zero
        case '^': return pow(a, b);
    }
    return 0;
}

// Function to convert infix to postfix
string infixToPostfix(const string &infix) {
    stack<char> st;
    string postfix;

    for (char ch : infix) {
        if (isspace(ch)) continue;

        if (isdigit(ch)) {
            postfix += ch;
            postfix += ' ';
        }
        else if (ch == '(') st.push(ch);
        else if (ch == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            st.pop(); // remove '('
        }
        else { // operator
            while (!st.empty() && precedence(st.top()) >= precedence(ch)) {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            st.push(ch);
        }
    }

    while (!st.empty()) {
        postfix += st.top();
        postfix += ' ';
        st.pop();
    }

    return postfix;
}

// Function to evaluate postfix expression
int evaluatePostfix(const string &postfix) {
    stack<int> st;
    int i = 0;

    while (i < postfix.length()) {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }

        if (isdigit(postfix[i])) {
            int num = 0;
            while (i < postfix.length() && isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            st.push(num);
        } else {
            int val2 = st.top(); st.pop();
            int val1 = st.top(); st.pop();
            char op = postfix[i];
            st.push(applyOp(val1, val2, op));
            i++;
        }
    }
    return st.top();
}

int main() {
    string infix;
    ifstream inputFile("expression_input.txt");
    ofstream outputFile("expression_output.txt");

    if (!inputFile) {
        cout << "Error: Could not open input file.\n";
        return 1;
    }
    if (!outputFile) {
        cout << "Error: Could not open output file.\n";
        return 1;
    }

    // Read infix expression from file
    getline(inputFile, infix);
    inputFile.close();

    cout << "Expression read from file: " << infix << endl;

    string postfix = infixToPostfix(infix);
    int result = evaluatePostfix(postfix);

    // Display output on console
    cout << "\nPostfix Expression: " << postfix << endl;
    cout << "Result: " << result << endl;

    // Write output to file
    outputFile << "Infix Expression: " << infix << endl;
    outputFile << "Postfix Expression: " << postfix << endl;
    outputFile << "Result: " << result << endl;

    outputFile.close();

    cout << "\nResults have been saved to 'expression_output.txt'." << endl;

    return 0;
}
