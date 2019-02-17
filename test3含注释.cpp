#include <iostream> 
#include <cstdlib> 
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
using namespace std;

enum Type { INT = 0, FLOAT, BIGINT };

int sign(int x) {
    return x >= 0 ? 1 : -1;
}

/** 构造 BigInt 类 
 * BigInt 类有两个构造函数：通过一个字符串（如：12321）和符号（1 代表正数，-1代表负数），或者通过属性 val 来直接构造
 * 属性 val 为将其数值【倒序】存储的动态数组（如：12345678987，存储为：{7,8,9,8,7,6,5,4,3,2,1}）
 */

class BigInt {
public: 
    BigInt(string str, int sign_);
    BigInt(vector<int> v):val(v) {
                                                                    cout << "bigint constructor2... " << endl;
        sign = val[val.size() - 1] >= 0 ? 1 : -1;
    }
    ~BigInt() {cout << "bigint destructor... " << endl;}
    friend BigInt intTObigint(int x);
    friend BigInt bigAdd(BigInt *x, BigInt *y);
    friend BigInt bigMultiply(BigInt *x, BigInt *y);
    friend BigInt bigPow(BigInt *x, int y);
public: 
    string strbigint;
    int sign;
    vector<int> val;
    static Type type;
};
Type BigInt::type = BIGINT;

BigInt::BigInt(string str, int sign_) {
                                                                cout << "bigint constructor1... " << endl;
    strbigint = str;
    sign = sign_;
    val.clear();
    for (int i = 0; i < str.length(); i++) {
        val.insert(val.begin(), sign * (str[i] - '0'));
    }
}

BigInt intTObigint(int x) {
                                                                cout << "call intTobigint... " << x << endl;
    vector<int> val;
    val.clear();
    int val_ = abs(x);
    while (val_) {
        val.push_back(sign(x) * (val_ % 10));
        val_ /= 10;
    }
    if (val.size() == 0) val.push_back(0);
    BigInt x_(val);
    return x_;
}

BigInt bigAdd(BigInt *x, BigInt *y) {
                                                                        cout << "call bigadd..." << endl;
    vector<int> sum;
    vector<int>::iterator iterx = x->val.begin();
    vector<int>::iterator itery = y->val.begin();
    while (iterx != x->val.end() && itery != y->val.end()) {
        sum.push_back(*iterx + *itery);
        iterx++;
        itery++;
    }
    while (iterx != x->val.end()) {
        sum.push_back(*iterx);
        iterx++;
    }
    while (itery != y->val.end()) {
        sum.push_back(*itery);
        itery++;
    }
    while (sum.size() && sum.back() == 0) 
        sum.pop_back();
    if (sum.size() == 0) sum.push_back(0);
    int up = 0;
    for (int i = 0; i < sum.size(); i++) {
        sum[i] += up;
        if ((sign(sum.back()) == 1 && sum[i] >= 10) || (sign(sum.back()) == -1 && sum[i] > 0)) {
            sum[i] -= 10;
            up = 1;
        }
        else if ((sign(sum.back()) == 1 && sum[i] < 0) || (sign(sum.back()) == -1 && sum[i] <= -10)) {
            sum[i] += 10;
            up = -1;
        }
        else up = 0;
    }
    if (up != 0) sum.push_back(up);
    while (sum.size() && sum.back() == 0) 
        sum.pop_back();
    if (sum.size() == 0) sum.push_back(0);
    BigInt res(sum);
    return res;
}

BigInt bigMultiply(BigInt *x, BigInt *y) {
    vector<int> zero;
    zero.push_back(0);
    BigInt mul(zero);
    int s1 = x->val.size(), s2 = y->val.size();
    for (int i = 0; i < s2; i++) {
        BigInt mid(zero);
        for (int j = 0; j < abs(y->val[i]); j++) {
            BigInt *pmid = new BigInt (mid);
            mid = bigAdd(x, pmid);
        }
        mid.val.insert(mid.val.begin(), i, 0);
        BigInt *pmid = new BigInt(mid);
        BigInt *pmul = new BigInt(mul);
        mul = bigAdd(pmul, pmid);
    }
    if (y->sign == -1) {
        for (int i = 0; i < mul.val.size(); i++) 
            mul.val[i] = -mul.val[i];
    }
    BigInt ans(mul.val);
    return ans;
}

BigInt bigPow(BigInt *x, int y) {
    vector<int> one_;
    one_.push_back(1);
    BigInt mul(one_);
    for (int i = 0; i < y; i++) {
        BigInt *pmul = new BigInt (mul);
        mul = bigMultiply(pmul, x);
    }
    return mul;
}

/** 构造 Value 类 
 * Value 类属性中有 BigInt 指针一个，初始为 NULL
 * Value 类有两个构造函数：通过已知属性 type 来直接构造；或者通过字符串和符号，首先确定 type，再构造
 * 对于第二个构造函数，若判断字符串为大整数，则新建一个 BigInt 类对象存储之，并将 pBigInt 指针指向这个对象
 * Value 类中定义了 getVal() 函数。对于每个 Value 对象有且仅有一个 type 属性，通过调用函数将其对应的 value 打印出来
 */

class Value {
public: 
    Value(Type tp) {
                                                        cout << "value constructor1..." << endl;
        type = tp;
        pBigInt = NULL;
    }
    Value(string str, int sign){
                                                        cout << "value constructor2..." << endl;
        pBigInt = NULL;
        strvalue = str;
        int i;
        for (i = 0; i < strvalue.length(); i++) {
            if (str[i] == '.' && i <= 10) {
                type = FLOAT;
                vFloat = sign * atof(strvalue.c_str());
                break;
            }
            else if (str[i] == '.' && i > 10) 
                throw runtime_error("type error");
            else if (i >= 10) {
                type = BIGINT;
                pBigInt = new BigInt(strvalue, sign);
                break;
            }
        }
        if (i == strvalue.length()) {
            type = INT;
            vInt = sign * atoi(strvalue.c_str());
        }
    }
    ~Value() {cout << "value destructor... " << endl;}
    friend Value operator + (Value x, Value y);
    friend Value operator * (Value x, Value y);
    friend Value operator / (Value x, Value y);
    friend Value operator ^ (Value x, Value y);
    void getVal();
private: 
    string strvalue;
    int vInt;
    float vFloat;
    BigInt *pBigInt;
    Type type;
};

void Value::getVal() {
                                                                cout << "call getVal..." << endl;
    if (type == INT)
        cout << vInt << endl;
    if (type == FLOAT)
        cout << vFloat << endl;
    if (type == BIGINT) {
        if (pBigInt->sign == -1) cout << "-";
        for (int i = pBigInt->val.size() - 1; i >= 0; i--) {
            cout << abs(pBigInt->val[i]);
        }
        cout << endl;
    }
}

////////// Overload Operators //////////

Value operator + (Value x, Value y) {
    if (x.type == INT && y.type == INT) {
        Value sum(INT);
        sum.vInt = x.vInt + y.vInt;
        return sum;
    }
    if (x.type == INT && y.type == FLOAT) {
        Value sum(FLOAT);
        sum.vFloat = x.vInt + y.vFloat;
        return sum;
    }
    if (x.type == INT && y.type == BIGINT) {
        Value sum(BIGINT);
        BigInt x_ = intTObigint(x.vInt);
        BigInt res = bigAdd(&x_, y.pBigInt);
        sum.pBigInt = new BigInt(res);
        return sum;
    }
    if (x.type == FLOAT && y.type == INT) {
        Value sum(FLOAT);
        sum.vFloat = x.vFloat + y.vInt;
        return sum;
    }
    if (x.type == FLOAT && y.type == FLOAT) {
        Value sum(FLOAT);
        sum.vFloat = x.vFloat + y.vFloat;
        return sum;
    }
    if (x.type == FLOAT && y.type == BIGINT) {
        throw runtime_error("unsupported operation: float +/- bigInt");
    }
    if (x.type == BIGINT && y.type == INT) {
        Value sum(BIGINT);
        BigInt y_ = intTObigint(y.vInt);
        BigInt res = bigAdd(&y_, x.pBigInt);
        sum.pBigInt = new BigInt(res);
        return sum;
    }
    if (x.type == BIGINT && y.type == FLOAT) {
        throw runtime_error("unsupported operation: float +/- bigInt");
    }
    if (x.type == BIGINT && y.type == BIGINT) {
        Value sum(BIGINT);
        BigInt res = bigAdd(x.pBigInt, y.pBigInt);
        sum.pBigInt = new BigInt(res);
        return sum;
    }
    else throw runtime_error("type error");
}

Value operator * (Value x, Value y) {
    if (x.type == INT && y.type == INT) {
        Value mul(INT);
        mul.vInt = x.vInt * y.vInt;
        return mul;
    }
    if (x.type == INT && y.type == FLOAT) {
        Value mul(FLOAT);
        mul.vFloat = x.vInt * y.vFloat;
        return mul;
    }
    if (x.type == INT && y.type == BIGINT) {
        Value mul(BIGINT);
        BigInt x_ = intTObigint(x.vInt);
        BigInt res = bigMultiply(&x_, y.pBigInt);
        mul.pBigInt = new BigInt(res);
        return mul;
    }
    if (x.type == FLOAT && y.type == INT) {
        Value mul(FLOAT);
        mul.vFloat = x.vFloat * y.vInt;
        return mul;
    }
    if (x.type == FLOAT && y.type == FLOAT) {
        Value mul(FLOAT);
        mul.vFloat = x.vFloat * y.vFloat;
        return mul;
    }
    if (x.type == FLOAT && y.type == BIGINT) {
        throw runtime_error("unsupported operation: float * bigInt");
    }
    if (x.type == BIGINT && y.type == INT) {
        Value mul(BIGINT);
        BigInt y_ = intTObigint(y.vInt);
        BigInt res = bigMultiply(&y_, x.pBigInt);
        mul.pBigInt = new BigInt(res);
        return mul;
    }
    if (x.type == BIGINT && y.type == FLOAT) {
        throw runtime_error("unsupported operation: float * bigInt");
    }
    if (x.type == BIGINT && y.type == BIGINT) {
        Value mul(BIGINT);
        BigInt res = bigMultiply(x.pBigInt, y.pBigInt);
        mul.pBigInt = new BigInt(res);
        return mul;
    }
    else throw runtime_error("type error");
}

Value operator / (Value x, Value y) {
    if (x.type == INT && y.type == INT) {
        if (x.vInt / y.vInt == (int)x.vInt / y.vInt) {
            Value div(INT);
            div.vInt = x.vInt / y.vInt;
            return div;
        }
        else {
            Value div(FLOAT);
            div.vFloat = x.vInt / y.vInt;
            return div;
        }
    }
    if (x.type == INT && y.type == FLOAT) {
        if (x.vInt / y.vFloat == (int)x.vInt / y.vFloat) {
            Value div(INT);
            div.vInt = x.vInt / y.vFloat;
            return div;
        }
        else {
            Value div(FLOAT);
            div.vFloat = x.vInt / y.vFloat;
            return div;
        }
    }
    if (x.type == FLOAT && y.type == INT) {
        Value div(FLOAT);
        div.vFloat = x.vFloat / y.vInt;
        return div;
    }
    if (x.type == FLOAT && y.type == FLOAT) {
        if (x.vFloat / y.vFloat == (int)x.vFloat / y.vFloat) {
            Value div(INT);
            div.vInt = x.vFloat / y.vFloat;
            return div;
        }
        else {
            Value div(FLOAT);
            div.vFloat = x.vFloat / y.vFloat;
            return div;
        }
    }
    if (x.type == BIGINT || y.type == INT) {
        throw runtime_error("unsupported operation: ... / bigInt");
    }
    else throw runtime_error("type error");
}

Value operator ^ (Value x, Value y) {
    if (x.type == INT && y.type == INT) {
        Value mul(INT);
        mul.vInt = pow(x.vInt, y.vInt);
        return mul;
    }
    if (x.type == FLOAT && y.type == INT) {
        Value mul(FLOAT);
        mul.vFloat = pow(x.vFloat, y.vInt);
        return mul;
    }
    if (x.type == BIGINT && y.type == INT) {
        Value mul(BIGINT);
        BigInt res = bigPow(x.pBigInt, y.vInt);
        mul.pBigInt = new BigInt(res);
        return mul;
    }
    if (y.type == FLOAT) {
        throw runtime_error("unsupported operation: ... ^ float");
    }
    if (y.type == BIGINT) {
        throw runtime_error("unsupported operation: ... ^ bigInt");
    }
    else throw runtime_error("type error");
}

/** 构造 Calculator 类 
 * input() 函数令用户输入一行算式（字符串），目前仅支持 整数、小数、大整数和 +、- 号，将输入存储为 ini_input
 * compute(int start, int end) 函数将计算 ini_input 中从第 start 个字符（包含）到第 end 个字符（不包含）的结果，方便之后递归使用
 * getResult() 函数将 result 指向 compute(0, ini_input.length()) 的结果
 * printResult() 函数通过调用指针 result 所指 Value 类对象的 getVal() 函数，将其 val 打印出来
 */

class Calculator {
public: 
    Calculator() {
        result = NULL;
    }
    ~Calculator() {};
    void input();
    Value compute(int start, int end);
    void getResult();
    void printResult();
protected: 
    string ini_input;
    Value *result;
    static vector<string> memstr;
    static vector<Value> memres;
};
vector<string> Calculator::memstr;
vector<Value> Calculator::memres;

void Calculator::input() {
    cout << ">> ";
    ini_input.clear();
    getline(cin, ini_input);
}

Value Calculator::compute(int start, int end) {
                                                                                cout << "call compute... " << endl;
    vector<Value> input;
    for (int i = start; i < end; ) {
        if (i == start && ini_input[i] == '-') i++;
        int j = i;
        while (j < end && ini_input[j] != '+' && ini_input[j] != '-') {
            j++;
        }
        if (i - 1 >= 0 && ini_input[i - 1] == '-') {
            Value in(ini_input.substr(i, j - i), -1);
            input.push_back(in);
        }
        else {
            Value in(ini_input.substr(i, j - i), 1);
            input.push_back(in);
        }
        i = j + 1;
    }
    for (int i = 1; i < input.size(); i++) {
        input[0] = input[0] + input[i];
    }
    Value res = input[0];
    input.clear();
    return res;
}

void Calculator::getResult() {
                                                                    cout << "call getResult... " << endl;
    Value ans = compute(0, ini_input.length());
    result = new Value(ans);
    memstr.push_back(ini_input);
    memres.push_back(ans);
}

void Calculator::printResult() { 
                                                                    cout << "call printResult... " << endl;
    result->getVal();
}

int main() {
    Calculator c;
    c.input();
    c.getResult();
    c.printResult();
    return 0;
}