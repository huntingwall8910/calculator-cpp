#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stack>
#include <regex>
#include <cctype>
#include <chrono> 
using db = double;
using namespace std;
//tokenize expression
vector<string> tokenizeExp(string str) {
    vector<string> tokens;
    regex r(R"([0-9.+*/()\- \t\r\n]+)");
    if (!regex_match(str, r)) return {};
    string temp;
    for (char c : str) {
        if (isspace(static_cast<unsigned char>(c))) continue;
        if (isdigit(static_cast<unsigned char>(c)) || c == '.') {
            temp += c;
        } else {
            if (!temp.empty()) {
                tokens.push_back(temp);
                temp.clear();
            }
            tokens.push_back(string(1, c));
        }
    }
    if (!temp.empty()) tokens.push_back(temp);
    return tokens;
}
//helper function
int prec(string s) {
    if (s == "*" || s == "/") return 2;
    if (s == "+" || s == "-") return 1;
    return 0;
}
//infix to postfix
vector<string> convPfix(vector<string> tokens){
    stack<string> op;
    vector<string> output;
    for (string token : tokens){
        if (isdigit(token[0]) || token[0] == '.'){
            output.push_back(token);
        }
        else if (token == "("){
            op.push(token);
        }
        else if (token == ")"){
            while (!op.empty() && op.top() != "("){
                output.push_back(op.top());
                op.pop();
            }
            op.pop();
        }
        else {
            while (!op.empty() && (prec(op.top()) >= prec(token))){
                output.push_back(op.top());
                op.pop();
            }
            op.push(token);
        }
    }
    while (!op.empty()){
        output.push_back(op.top());
        op.pop();
    }
    return output;
}
db evalPfix(vector<string> tokens){
    stack<db> s;
    for (string token : tokens){
        if (isdigit(token[0]) || token[0] == '.'){
            s.push(stod(token));
        }
        else {
            db y = s.top(); s.pop();
            db x = s.top(); s.pop();
            if (token == "+") s.push(x + y);
            else if (token == "-") s.push(x - y);
            else if (token == "*") s.push(x * y);
            else if (token == "/") s.push(x / y); 
        }
    }
    return s.top();
}
int main(){
    cout << "Calculator: Basic operations (+,-,*,/) with parentheses" << '\n' << "(doesnt check for errors or negative numbers bc i'm not coding allat)" << '\n';
    while (true){
        cout << "Enter your Expression: ";
        string exp;
        getline(cin,exp);
        auto start = chrono::high_resolution_clock::now();
        cout << setprecision(15) << evalPfix(convPfix(tokenizeExp(exp))) << '\n';
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "Calculated in " << duration.count() << " microseconds" << '\n';
    }
    return 0;
}
