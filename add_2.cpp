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
    if (n == 0)
        return -1;
    while (pow(i, n) <= result) {
        if (pow(i, n) == result) {
            return i;
        }
        if (pow(-i, n) == result) {
            return -i;
        }
        i += 1;
    }
    return -1;
}

int FindIndex(string c){
    for(int i=0; i< 12; i++)
        if (OpMap[i].first == c)
            return i;
    return -1;
}

int find(string s, char c){
    for(int i=0; i< s.length(); i++)
        if (s[i] == c)
            return i;
    return -1;
}

int isNumber(string s){
    int DotNumber = 0;
    for(int i=0; i<s.length(); i++){
        if(s[i] == '.')
            DotNumber++;
        else if(!isdigit(s[i]))
            return 0;
        else continue;
    }
    if(DotNumber > 1)
        return 0;
    return 1;
}

vector<string> ConvertToInfix(string origin){
    vector<string> converted = {};
    string temp = "";
    int BracketNumber = 0;
    for(int i=0; i < origin.length(); i++){
        string operand = string(1, origin[i]);
        if (origin[i] == ' ')
            continue;
        if (FindIndex(operand) < 0) {
            i++;
            while(FindIndex(string(1, origin[i])) < 0 && i < origin.length())
                operand += origin[i++];
            //cout << operand << endl;
            if (FindIndex(operand) >= 0) {
                if(origin[i] != '('){
                    cout << "Wrong expression!" << endl;
                    return {"Expression is wrong"};
                }
                converted.push_back(operand);
            }
            else{
                if (isNumber(operand) == 1) {
                    if(origin[i] == '(') {
                        cout << "Operator is missing between operand and bracket! 1" << endl;
                        return {"Expression is wrong"};
                    }
                    temp += operand;
                }
                else{
                    cout << "Wrong word is included!" << endl;
                    return {"Expression is wrong"};
                }
            }
            i--;
        }
        else{
            int index = FindIndex(operand);
            if (string::npos != temp.find_first_of("0123456789")) {
                converted.push_back(temp);
                temp = "";
            }
            if (origin[i] == '-' && (i == 0 || origin[i - 1] == '(')) {
                converted.push_back("0");
            }
            else {
                if ((i == 0 && origin[i] != '(') || (i == origin.length()-1 && origin[i] != ')')){
                    cout << "Grammar violated! 2" << endl;
                    return {"Expression is wrong"};
                }
                if (i > 0 && i < origin.length()-1) {
                    /*if (((origin[i] == '(' && (isdigit(origin[i-1])) || (FindIndex(string(1, origin[i+1])) >= 0 && origin[i+1] != '-')))
                    || ((origin[i] == ')' && (isdigit(origin[i+1])) || (FindIndex(string(1, origin[i-1])) >= 0 && origin[i-1] != '-')))){
                        cout << origin[i] << " " << origin[i+1] << " " << " Grammar violated! 3" << endl;
                        return {"Expression is wrong"};
                    }*/
                    if (origin[i] == '(' && isdigit(origin[i-1])){
                        cout << "Grammar violated! 3" << endl;
                        return {"Expression is wrong"};
                    }
                    if (origin[i] == '(' && FindIndex(string(1, origin[i+1])) >= 0 && origin[i+1] != '(' && origin[i+1] != '-'){
                        cout << "Grammar violated! 4" << endl;
                        return {"Expression is wrong"};
                    }
                    if (origin[i] == ')' && isdigit(origin[i+1])){
                        cout << "Grammar violated! 5" << endl;
                        return {"Expression is wrong"};
                    }
                    if (origin[i] == ')' && FindIndex(string(1, origin[i-1])) >= 0 && origin[i-1] != ')'){
                        cout << "Grammar violated! 6" << endl;
                        return {"Expression is wrong"};
                    }
                    if (FindIndex(string(1, origin[i])) >= 0 && FindIndex(string(1, origin[i])) <= 4
                    && FindIndex(string(1, origin[i-1])) >= 0 && FindIndex(string(1, origin[i-1])) <= 4) {
                        cout << origin[i] << " " << origin[i-1] << " " << " Grammar violated! 7" << endl;
                        return {"Expression is wrong"};
                    }
                }
                if (origin[i] == '(')
                    BracketNumber ++;
                if (origin[i] == ')'){
                    BracketNumber --;
                    if (BracketNumber < 0){
                        cout << "Brackets are placed incorrectly! 4" << endl;
                        return {"Expression is wrong"};
                    }
                }
            }
            converted.push_back(operand);
        }
    }
    if (temp != "")
        converted.push_back(temp);
    if (BracketNumber != 0){
        cout << "Brackets are placed incorrectly! 5" << endl;
        return {"Expression is wrong"};
    }
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
            if (OpStorage.size() > 0 && OpStorage.back() == "sqrt"){
                postfix.push_back(OpStorage.back());
                OpStorage.pop_back();
            }
        }
        else {
            if (infix[i] != ",") {
                while (OpStorage.size() > 0 && OpMap[FindIndex(infix[i])].second <= OpMap[FindIndex(OpStorage.back())].second) {
                    postfix.push_back(OpStorage.back());
                    OpStorage.pop_back();
                }
                OpStorage.push_back(infix[i]);
            }
        }
        /*for(int i=0; i<postfix.size(); i++)
            cout << postfix[i];
        cout << endl;
        for(int i=0; i<OpStorage.size(); i++)
            cout << OpStorage[i];
        cout << endl;
        cout << infix[i] << endl;*/
    }
    return postfix;
}

float calculate(vector<string> & postfix, int &solved)
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
                    if (n1 == 0){
                        cout << "Unable to calculate!" << endl;
                        return 0;
                    }
                    n2 = pow(n2, 1/n1);
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
    vector<string> ConvertedExpression = ConvertToInfix(expression);
    /*for(int i = 0; i< ConvertedExpression.size(); i++)
        cout << ConvertedExpression[i] << endl;
    cout << endl;*/
    if (ConvertedExpression[0] != "Expression is wrong") {
        int solved = 1;
        ConvertedExpression = InfixToPostfix(ConvertedExpression);
        /*for (int i = 0; i < ConvertedExpression.size(); i++)
            cout << ConvertedExpression[i] << endl;
        cout << endl;*/
        float result = calculate(ConvertedExpression, solved);
        if (solved == 1)
            cout << result << endl;
    }
}

/* -2 + 10 * (20.1 - 0.1) ^ 2 + sin(0)/1
 -(-1)
-(0+1)
-sin(1)
 */
