#include <iostream>
#include <iomanip>
#include <string>
#include <vector> 
#include <stack>
#include <cmath>
#include "Exception.h"
#define PI M_PI
using namespace std;

const char HELP[] = 
    "Commands:\n"
    ">> help\t\t\t# show command list\n"
	">> set precision 3\t# e.g. 1.0/2 = 0.500 (default = 3)\n"
    ">> pi\t\t\t# calculated PI in cmath\n"
    ">> e\t\t\t# calculated exp in cmath\n"
    ">> ans\t\t\t# get previous result\n"
    ">> function\t\t# show function list\n"
    ">> set degree\t\t# e.g. sin(30) = 0.5\n"
	">> set radian\t\t# e.g. sin(30) = -0.988 (by default)\n"
    ">> delete\t\t# delete last result from memory (update ans)\n"
    ">> show\t\t\t# show all results saved\n"
    ">> reset\t\t# reset the calculator\n"
    ">> quit\t\t\t# quit the calculator\n";

const char FUNCTION[] = 
    "Functions avaliable:\n"
    ">> cos()\n"
    ">> sin()\n"
    ">> tg()\n"
    ">> ctg()\n"
    ">> abs()\n"
    ">> sign()\n"
    ">> sqrt()\n"
    ">> ln()\n"
    ">> sinh()\n"
    ">> cosh()\n"
    ">> tanh()\n"
    ">> coth()\n";

struct Element {
    float val;
    bool isVal;
};
typedef struct Element Node;

void addNode(vector<Node> &arr, float v, bool isV) {
    Node node;
    node.val = v;
    node.isVal = isV;
    arr.push_back(node);
}

class Calculator {
public: 
    Calculator(){};
    ~Calculator(){}
    void input(); 

    // about commands:
    bool isCommand();
    void putHelp();
    void putFunction();
    void setPrecision(int m);

    // about computing: 
    vector<Node>contructLulipro();
    float compute();
    float getLastResult();
    void getThisResult();
    void printResult();
    float luliproAlgo(int start, int end);

    // about memory: 
    void addMemory();
    void showMemory();
    void popMemory();
    void clearMemory();
private: 
    string ini_input;
    float result;
    static int precision;
    static bool setRadian;
    static vector<string> memstr;
    static vector<float> memresult;
    static vector<string> membig;
};

int Calculator::precision = 3;
bool Calculator::setRadian = 1;
vector<string> Calculator::memstr;
vector<float> Calculator::memresult;

void Calculator::input() {
    this->ini_input.clear();
    getline(cin, this->ini_input);
}

void Calculator::putHelp() {
    puts(HELP);
}

void Calculator::putFunction() {
    puts(FUNCTION);
}

void Calculator::setPrecision(int m) {
    precision = m;
}

float Calculator::getLastResult() {
    return memresult.back();
}

void Calculator::addMemory() {
    memstr.push_back(ini_input);
    memresult.push_back(result);
}

void Calculator::showMemory() {
    for (int i = 0; i < memstr.size(); i++)
        cout << memstr[i] << " = " << memresult[i] << endl;
}

void Calculator:: popMemory() {
    memstr.pop_back();
    memresult.pop_back();
}

void Calculator:: clearMemory() {
    memstr.clear();
    memresult.clear();
}

////////// isCommand //////////

bool Calculator::isCommand() {
    if (ini_input.substr(0, 4) == "help") {
        putHelp();
        return true;
    }
    else if (ini_input.substr(0, 8) == "function") {
        putFunction();
        return true;
    }
    else if (ini_input.substr(0, 13) == "set precision") { 
        string temp = ini_input.substr(14, ini_input.length());
        int m = 0;
        for (int i = 0; i < temp.length(); i++) {
            m += temp[i] - '0';
            m *= 10;
        }
        m /= 10;
        setPrecision(m);
        return true;
    }
    else if (ini_input.substr(0, 10) == "set degree") {
        setRadian = 0;
        return true;
    }
    else if (ini_input.substr(0, 10) == "set radian") {
        setRadian = 1;
        return true;
    }
    else if (ini_input.substr(0, 6) == "delete") {
        popMemory();
        return true;
    }
    else if (ini_input.substr(0, 4) == "show") {
        showMemory();
        return true;
    }
    else if (ini_input.substr(0, 5) == "reset") {
        clearMemory();
        setPrecision(3);
        setRadian = 1;
        return true;
    }
    else if (ini_input.substr(0, 4) == "quit") {
        exit(1);
    }
    else return false;
}

////////// lulipro algorithm //////////

float power(int count) {
    return count == 0 ? 1 : 0.1 * power(count - 1);
}

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

float charToval(char c) {
    return c - '0';
}

int ComputeLevel(char c) {
    if (c == '+') return 1;
    else if (c == '-') return 2;
    else if (c == '*') return 3;
    else if (c == '/') return 4;
    else return 5;
}

float factorial(float a, int b) {
    return b == 0 ? 1 : a * factorial(a, b - 1);
}

string findFunction(string str, int i) {
    const string functions[13] = {"cos", "sin", "tg", "ctg", "abs", "sign", "sqrt", "ln", "sinh", "cosh", "tanh", "coth", "exp"};
    for (int j = 0; j < 13; j++) {
        if (str.substr(i, functions[j].length()) == functions[j])
            return functions[j];
    }
    return "0";
}

float funcCompute(string func, float x, bool setRadian_) {
    if (func == "sin")
        return setRadian_ ? sin(x) : sin(x / 180 * PI);
    else if (func == "cos")
        return setRadian_ ? cos(x) : cos(x / 180 * PI);
    else if (func == "tg") 
        return setRadian_ ? tan(x) : tan(x / 180 * PI);
    else if (func == "ctg")
        return setRadian_ ? 1 / tan(x) : 1 / tan(x / 180 * PI);
    else if (func == "abs")
        return abs(x);
    else if (func == "sign")
        return x > 0 ? 1 : -1;
    else if (func == "ln")
        return log(x);
    else if (func == "sinh")
        return sinh(x);
    else if (func == "cosh")
        return cosh(x);
    else if (func == "tanh")
        return tanh(x);
    else if (func == "coth")
        return 1 / tanh(x);
    else 
        return exp(x);
}

float Calculator::luliproAlgo(int start, int end) {
    stack<char> SOP;
    vector<Node> luliproArr;
    int count = 0;
    for (int i = start; i < end; i++) {
        if (isNumber(this->ini_input[i])) {
            if (i != start && (isNumber(this->ini_input[i - 1]) || this->ini_input[i - 1] == '.')) {
                if (!count)
                    luliproArr.back().val = luliproArr.back().val * 10 + charToval(this->ini_input[i]);
                else 
                    luliproArr.back().val += charToval(this->ini_input[i]) * power(count++);
            }
            else 
                addNode(luliproArr, charToval(this->ini_input[i]), 1);
        }
        else if (this->ini_input[i] == '.') {
            count++;
        }
        else if (isOperator(this->ini_input[i]) || this->ini_input[i] == '(') {
            count = 0;
            if (i == 0 && this->ini_input[i] == '-') {
                addNode(luliproArr, 0, 1);
            }
            if (SOP.size()) {
                while (SOP.size() && SOP.top() != '(' && ComputeLevel(SOP.top()) > ComputeLevel(this->ini_input[i])) {
                    addNode(luliproArr, SOP.top(), 0);
                    SOP.pop();
                }
            }
            SOP.push(this->ini_input[i]);
        }
        else if (this->ini_input[i] == ')'){
            count = 0;
            while (SOP.top() != '(') {
                addNode(luliproArr, SOP.top(), 0);
                SOP.pop();
            }
            SOP.pop();
        }
        else if (this->ini_input.substr(i, 3) == "ans"){
            addNode(luliproArr, getLastResult(), 1);
            i += 2;
        }
        else if (this->ini_input.substr(i, 2) == "pi") {
            addNode(luliproArr, PI, 1);
            i++;
        }
        else if (this->ini_input[i] == 'e') {
            addNode(luliproArr, exp(1), 1);
        }
        else if (findFunction(this->ini_input, i) != "0") {
            int j = i + 1, num = 0;
            while (this->ini_input[j] != ')') {
                if (this->ini_input[j] == '(') num++;
                j++;
            } 
            while (num > 1) {
                j++;
                if (this->ini_input[j] == ')') num--;
            }
            Node node;
            node.val = funcCompute(findFunction(this->ini_input, i), (luliproAlgo(i + findFunction(this->ini_input, i).length() + 1, j)), setRadian);
            node.isVal = 1;
            luliproArr.push_back(node);
            i = j;
        }
        else 
            throw TypeError(this->ini_input, i);
    }
    while (!SOP.empty()) {
        Node node;
        node.val = SOP.top();
        node.isVal = 0;
        luliproArr.push_back(node);
        SOP.pop();
    }
    // compute: 
    stack<float> temp;
    for (int i = 0; i < luliproArr.size(); i++) {
        if (luliproArr[i].isVal) temp.push(luliproArr[i].val);
        else {
            float val = temp.top();
            temp.pop();
            switch ((char)luliproArr[i].val) {
                case '+': {
                    temp.top() += val;
                    break;
                }
                case '-': {
                    temp.top() -= val;
                    break;
                }
                case '*': {
                    temp.top() *= val;
                    break;
                }
                case '/': {
                    temp.top() /= val;
                    break;
                }
                case '^': {
                    temp.top() = factorial(temp.top(), val);
                }
            }
        }
    }
    return temp.top();
}

////////// get results //////////

void Calculator::getThisResult() {
    this->result = luliproAlgo(0, ini_input.length());
}

void Calculator::printResult() {
    cout << setiosflags(ios::fixed) << setprecision(this->precision) << this->result << endl;
    addMemory();
}

////////// test //////////

void loop() {
    Calculator c;
    do {
        cout << ">> ";
        c.input();
    } while (c.isCommand());
    c.getThisResult();
    c.printResult();
}

int main() {
    puts(HELP);
    while (true) {
        try {
            loop();
        } catch (Exception &theException) {
            theException.printError();
        }
    }
    return 0;
}