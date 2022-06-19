#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <string.h>
#include <cstdlib>
#include <algorithm>
#include <math.h>
using namespace std;

vector<pair<string, int>> OpMap = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"sin", 4}, {"cos", 4}, {"tg", 4}, {"sqrt", 4}, {"(", -1}, {")", -1}, {",", 0}};

int getNthRoot(int result, int n) {
    int i = 1;
    while (pow(i, n) <= result) {
        if (pow(i, n) == result) {
            return i;
        }
        i += 1;
    }
}

int FindIndex(string c){
    for(int i=0; i< 12; i++)
        if (OpMap[i].first == c)
            return i;
    return -1;
}

vector<string> ConvertToInfix(string origin){
    vector<string> converted = {};
    string temp = "";
    for(int i=0; i < origin.length(); i++){
        string operand = string(1, origin[i]);
        if (origin[i] == ' ')
            continue;
        switch (origin[i]){
            case 's':
                if (origin[i+1] == 'i'){
                    operand = origin.substr(i, 3);
                    i += 2;
                }
                if (origin[i+1] == 'q'){
                    operand = origin.substr(i, 4);
                    i += 3;
                }
                break;
            case 'c':
                operand = origin.substr(i, 3);
                i += 2;
                break;
            case 't':
                operand = origin.substr(i, 2);
                i += 1;
                break;
            default:
                break;
        }
        if (FindIndex(operand) >= 0){
            int index = FindIndex(operand);
            if (string::npos != temp.find_first_of("0123456789"))
                converted.push_back(temp);
            if (origin[i] == '-') {
                if (i == 0)
                    temp = "-";
                else if (origin[i - 1] == '(') {
                    int j = i + 1;
                    for (; origin[j] != ')'; j++) {
                        if (!isdigit(origin[j]) && origin[j] != '.') {
                            temp = "-";
                            break;
                        }
                    }
                    if (origin[j] == ')') {
                        converted.pop_back();
                        temp = "-";
                        origin[j] = ' ';
                    }
                }
                else{
                    temp = "";
                    converted.push_back(operand);
                }
            }
            else{
                temp = "";
                converted.push_back(operand);
            }
        }
        else temp += origin[i];
    }
    if (temp != "")
        converted.push_back(temp);
    return converted;
}

vector<string> InfixToPostfix(vector<string> &infix){
    vector<string> postfix = {};
    vector<string> OpStorage = {"("};
    infix.push_back(")");
    for(int i=0; i<infix.size(); i++)
    {
        if(FindIndex(infix[i]) < 0)
            postfix.push_back(infix[i]);
        else if(infix[i] == "(")
            OpStorage.push_back(infix[i]);
        else if (infix[i] == ")") {
            while(OpStorage.back() != "("){
                postfix.push_back(OpStorage.back());
                OpStorage.pop_back();
            }
            OpStorage.pop_back();
        }
        else {
            if (infix[i] != ",") {
                while (OpStorage.size() > 0 &&
                       OpMap[FindIndex(infix[i])].second <= OpMap[FindIndex(OpStorage.back())].second) {
                    postfix.push_back(OpStorage.back());
                    OpStorage.pop_back();
                }
                OpStorage.push_back(infix[i]);
            }
        }
    }
    return postfix;
}

float calculate(vector<string> & postfix)
{
    vector<float> CalculationStack = {};
    for(int i=0; i<postfix.size(); i++) {
        if(FindIndex(postfix[i]) >= 0) {
            float n1, n2;
            if (postfix[i].length() > 1) {
                if (postfix[i] == "sin") {
                    n2 = sin(CalculationStack.back());
                    CalculationStack.pop_back();
                    CalculationStack.push_back(n2);
                }
                if (postfix[i] == "cos") {
                    n2 = cos(CalculationStack.back());
                    CalculationStack.pop_back();
                    CalculationStack.push_back(n2);
                }
                if (postfix[i] == "tg") {
                    n2 = tan(CalculationStack.back());
                    CalculationStack.pop_back();
                    CalculationStack.push_back(n2);
                }
                if (postfix[i] == "sqrt") {
                    n1 = CalculationStack.back();
                    n2 = CalculationStack[CalculationStack.size() - 2];
                    n2 = getNthRoot(n2, n1);
                    CalculationStack.pop_back();
                    CalculationStack.pop_back();
                    CalculationStack.push_back(n2);
                }
            }
            else {
                n1 = CalculationStack.back();
                n2 = CalculationStack[CalculationStack.size() - 2];
                switch (postfix[i][0]) {
                    case '+':
                        n2 = n2 + n1;
                        break;
                    case '-':
                        n2 = n2 - n1;
                        break;
                    case '*':
                        n2 = n2 * n1;
                        break;
                    case '/':
                        n2 = n2 / n1;
                        break;
                    case '^':
                        n2 = pow(n2, n1);
                        break;
                    default:
                        break;
                }
                CalculationStack.pop_back();
                CalculationStack.pop_back();
                CalculationStack.push_back(n2);
            }
        }
        else {
            CalculationStack.push_back(atof(postfix[i].c_str()));
        }
    }
    return CalculationStack[0];
}

int main(){
    string expression;
    getline(cin, expression);
    expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
    cout << expression << endl;
    vector<string> ConvertedExpression = ConvertToInfix(expression);
    ConvertedExpression = InfixToPostfix(ConvertedExpression);
    /*for(int i = 0; i< ConvertedExpression.size(); i++)
        cout << ConvertedExpression[i] << endl;*/
    float result = calculate(ConvertedExpression);
    cout << result << endl;
}

// -2 + 10 * (20.1 - 0.1) ^ 2 + sin(0)/1