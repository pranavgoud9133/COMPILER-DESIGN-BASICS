#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
using namespace std;

class ExpressionEvaluator {
public:
    ExpressionEvaluator(const string& expr) : expression(expr), pos(0) {}

    double evaluate() {
        double result = parseExpression();
        if (pos < expression.length()) {
            throw runtime_error("Unexpected character: " + string(1, expression[pos]));
        }
        return result;
    }

private:
    string expression;
    size_t pos;

    // Skip whitespaces
    void skipSpaces() {
        while (pos < expression.length() && isspace(expression[pos])) {
            pos++;
        }
    }

    // Parse numbers and handle parentheses
    double parseFactor() {
        skipSpaces();

        if (pos < expression.length() && expression[pos] == '(') {
            pos++;  // Skip '('
            double result = parseExpression();
            if (pos >= expression.length() || expression[pos] != ')') {
                throw runtime_error("Missing closing parenthesis");
            }
            pos++;  // Skip ')'
            return result;
        }

        bool negative = false;
        if (pos < expression.length() && expression[pos] == '-') {
            negative = true;
            pos++;
        }

        skipSpaces();
        double number = 0;
        bool decimal = false;
        double fraction = 0.1;

        if (pos >= expression.length() || (!isdigit(expression[pos]) && expression[pos] != '.')) {
            throw runtime_error("Expected number at position " + to_string(pos));
        }

        while (pos < expression.length() && (isdigit(expression[pos]) || expression[pos] == '.')) {
            if (expression[pos] == '.') {
                decimal = true;
            } else {
                if (!decimal) {
                    number = number * 10 + (expression[pos] - '0');
                } else {
                    number = number + (expression[pos] - '0') * fraction;
                    fraction /= 10;
                }
            }
            pos++;
        }

        return negative ? -number : number;
    }

    // Handle multiplication and division
    double parseTerm() {
        double result = parseFactor();
        skipSpaces();

        while (pos < expression.length()) {
            char op = expression[pos];
            if (op != '*' && op != '/') break;

            pos++;
            double nextFactor = parseFactor();
            if (op == '*') result *= nextFactor;
            else {
                if (nextFactor == 0) throw runtime_error("Division by zero");
                result /= nextFactor;
            }
            skipSpaces();
        }

        return result;
    }

    // Handle addition and subtraction
    double parseExpression() {
        double result = parseTerm();
        skipSpaces();

        while (pos < expression.length()) {
            char op = expression[pos];
            if (op != '+' && op != '-') break;

            pos++;
            double nextTerm = parseTerm();
            if (op == '+') result += nextTerm;
            else result -= nextTerm;
            skipSpaces();
        }

        return result;
    }
};

int main() {
    cout << "=== CodTech Task 4: Simple Arithmetic Expression Compiler ===" << endl;
    string input;

    while (true) {
        cout << "\nEnter expression (or type 'exit' to quit): ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            ExpressionEvaluator evaluator(input);
            double result = evaluator.evaluate();
            cout << "Result = " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
