#ifndef CALCULATOR_CALCULAT_H
#define CALCULATOR_CALCULAT_H

#include<iostream>
#include<algorithm>
#include<stack>
#include<vector>
#include<string>

#endif //CALCULATOR_CALCULAT_H

using namespace std;

class Calculator {
private:
    string str;

    bool checkValid() {
        stack<char> brackets;
        char lastChar = ' ';
        vector<char> chars(str.begin(), str.end());
        // expression cannot be empty
        if (chars.size() == 0)
            return false;
        for (int i = 0; i < chars.size(); i++) {
            switch (chars[i]) {
                case '(':
                    brackets.push(chars[i]);
                    break;
                case ')':
                    // check bracket pairing
                    if (brackets.empty())
                        return false;
                    if (brackets.top() != '(') {
                        return false;
                    }
                    brackets.pop();
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                    // operators should not be at the start or end
                    if (i == 0 || i == chars.size() - 1)
                        return false;
                    // operators themselves & left brackets should not be consecutive
                    if (chars[i - 1] == '+' || chars[i - 1] == '-' || chars[i - 1] == '*' || chars[i - 1] == '/' || chars[i - 1] == '(')
                        return false;
                    // operators themselves & right brackets should not be consecutive
                    if (chars[i + 1] == '+' || chars[i + 1] == '-' || chars[i + 1] == '*' || chars[i + 1] == '/' || chars[i + 1] == ')')
                        return false;
                    // save character of last iteration to check 0 division
                    lastChar = chars[i];
                    break;
                case '0':
                    if (lastChar == '/')
                        return false;
                    break;
                default:
                    lastChar = chars[i];
                    break;
            }
        }
        // stack should be empty after check
        return brackets.empty();
    }

    void format() {
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '-') {
                if (i == 0) {
                    str.insert(0, 1, '0');
                } else if (str[i - 1] == '(') {
                    str.insert(i, 1, '0');
                }
            }
        }
    }

    int priority(char c) {
        if (c == '+' || c == '-') {
            return 1;
        } else if (c == '*' || c == '/') {
            return 2;
        } else {
            return 0;
        }
    }

    vector<string> transition() {
        vector<string> postfix;
        stack<char> stack;
        for (int i = 0; i < str.length(); i++) {
            string tmp = "";
            switch (str[i]) {
                case '+':
                case '-':
                case '*':
                case '/':
                    if (stack.empty() || stack.top() == '(') {
                        stack.push(str[i]);
                    } else {
                        while (!stack.empty() && priority(stack.top()) >= priority(str[i])) {
                            tmp += stack.top();
                            postfix.push_back(tmp);
                            stack.pop();
                            tmp = "";
                        }
                        stack.push(str[i]);
                    }
                    break;
                case '(':
                    stack.push(str[i]);
                    break;
                case ')':
                    while (stack.top() != '(') {
                        tmp += stack.top();
                        postfix.push_back(tmp);
                        stack.pop();
                        tmp = "";
                    }
                    stack.pop();
                    break;
                default:
                    if ((str[i] >= '0' && str[i] <= '9')) {
                        tmp += str[i];
                        while (i + 1 < str.size() && str[i + 1] >= '0' && str[i + 1] <= '9' ||
                               str[i + 1] == '.') {
                            tmp += str[i + 1]; // consecutive numbers
                            ++i;
                        }
                        postfix.push_back(tmp);
                    }
            }
        }
        while (!stack.empty()) {
            string tmp = "";
            tmp += stack.top();
            postfix.push_back(tmp);
            stack.pop();
        }
        return postfix;
    }

    double getResult(vector<string> postfix) {
        stack<double> stack;
        double num, op1, op2;
        for (int i = 0; i < postfix.size(); i++) {
            string tmp = postfix[i];
            if (tmp[0] >= '0' && tmp[0] <= '9') {
                num = atof(tmp.c_str());
                stack.push(num);
            } else {
                op2 = stack.top();
                stack.pop();
                op1 = stack.top();
                stack.pop();
                if (postfix[i] == "+") {
                    stack.push(op1 + op2);
                } else if (postfix[i] == "-") {
                    stack.push(op1 - op2);
                } else if (postfix[i] == "*") {
                    stack.push(op1 * op2);
                } else if (postfix[i] == "/") {
                    stack.push(op1 / op2);
                }
            }
        }
        return stack.top();
    }

public:
    Calculator(string str) : str(str) {}

    string solve() {
        format();
        if (checkValid()) {
            vector<string> postfix = transition();
            return to_string(getResult(postfix));
        } else {
            return "Error";
        }
    }
};

