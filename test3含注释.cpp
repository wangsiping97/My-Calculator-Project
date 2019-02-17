#include <iostream> 
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
using namespace std;

enum Type { INT = 0, FLOAT, BIGINT };

/** 构造 Int, Float, BigInt 类 
 * 每个类有两个构造函数：通过一个字符串（如：12321）和符号（1 代表正数，-1代表负数），或者通过属性 val 来直接构造
 * 对于 Int 类和 Float 类，属性 val 为其数值
 * 对于 BigInt 类，属性 val 为将其数值【倒序】存储的动态数组（如：12345678987，存储为：{7,8,9,8,7,6,5,4,3,2,1}）
 */

class Int {
public: 
    Int(string str, int sign_) {
                                                cout << "int constructor1... " << endl;
        val = 0;
        strint = str;
        sign = sign_;
        val += str[0] - '0';
        int i = 1;
        while (i < str.length()) {
            val *= 10;
            val += str[i] - '0';
            i++;
        }
        val = sign * val;
                                                cout << "val: " << val << endl;
    }
    Int(int v):val(v) {
                                                cout << "int constructor2... " << v << endl;
        sign = val >= 0 ? 1 : -1;
    }
    ~Int() {cout << "int desstructor... " << endl;}
public: 
    string strint;
    int sign;
    int val;
    static Type type;
};
Type Int::type = INT;

class Float {
public: 
    Float(string str, int sign) {
        strfloat = str;
        val = 0;
        int i = 0; 
        while (str[i] != '.') {
            val += str[i] - '0';
            val *= 10;
            i++;
        }
        val /= 10; i++;
        int count = 1;
        while (i < str.length()) {
            val += (str[i] - '0') * pow(10, -count);
            count++;
            i++;
        }
        val = sign * val;
    }
    Float(float v):val(v) {}
    ~Float() {}
public: 
    string strfloat;
    float val;
    static Type type;
};
Type Float::type = FLOAT;

class BigInt {
public: 
    BigInt(string str, int sign_) {
                                                                    cout << "bigint constructor1... " << endl;
        strbigint = str;
        sign = sign_;
        val.clear();
        for (int i = 0; i < str.length(); i++) {
            val.insert(val.begin(), sign * (str[i] - '0'));
        }
                                                                    cout << "val(倒序): ";
                                                                    for (int i = 0; i < val.size(); cout << val[i], i++);
                                                                    cout << endl;
    }
    BigInt(vector<int> v):val(v) {
                                                                    cout << "bigint constructor2... " << endl;
        sign = val[val.size() - 1] > 0 ? 1 : -1;
    }
    ~BigInt() {cout << "bigint destructor... " << endl;}
    friend BigInt bigAdd(BigInt x, BigInt y);
public: 
    string strbigint;
    int sign;
    vector<int> val;
    static Type type;
};
Type BigInt::type = BIGINT;

/** 构造 Value 类 
 * Value 类属性中有 Int, Float, BigInt 指针各一个，初始为 NULL
 * Value 类有两个构造函数：通过已知属性 type 来直接构造；或者通过字符串和符号，首先确定 type，再构造
 * 对于第二个构造函数，若判断字符串为整数，则新建一个 Int 类对象存储之，并将 Int 指针指向这个对象。以此类推。
 * Value 类中定义了 getVal() 函数。对于每个 Value 类有且仅有一个非空指针，通过调用函数将其所指对象的 val 打印出来
 */

class Value {
public: 
    Value(Type tp) {
                                                        cout << "value constructor1..." << endl;
        type = tp;
        pInt = NULL;
        pFloat = NULL;
        pBigInt = NULL;
    }
    Value(string str, int sign){
                                                        cout << "value constructor2..." << endl;
        pInt = NULL;
        pFloat = NULL;
        pBigInt = NULL;
        strvalue = str;
        int i;
        for (i = 0; i < strvalue.length(); i++) {
            if (str[i] == '.' && i <= 10) {
                type = FLOAT;
                pFloat = new Float(strvalue, sign);
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
            pInt = new Int(strvalue, sign);
        }
    }
    ~Value() {cout << "value destructor... " << endl;}
    friend Value operator + (Value x, Value y);
    void getVal();
// private: 
    string strvalue;
    Int *pInt;
    Float *pFloat;
    BigInt *pBigInt;
    Type type;
};

void Value::getVal() {
                                                                cout << "call getVal..." << endl;
    if (pInt)
        cout << pInt->val << endl;
    else if (pFloat)
        cout << pFloat->val << endl;
    else if (pBigInt) {
        if (pBigInt->sign == -1) cout << "-";
        for (int i = pBigInt->val.size() - 1; i >= 0; i--) {
            cout << abs(pBigInt->val[i]);
        }
        cout << endl;
    }
}

////////// Overload Operators //////////

BigInt intTObigint(Int *x) {
                                                                cout << "call intTobigint... " << x->val << endl;
    vector<int> val;
    int val_ = abs(x->val);
    while (val_) {
        val.push_back(x->sign * (val_ % 10));
        val_ /= 10;
    }
    BigInt x_(val);
                                                                cout << "val(倒序): ";
                                                                for (int i = 0; i < val.size(); cout << val[i], i++);
                                                                cout << endl;
    return x_;
}

BigInt bigAdd(BigInt *x, BigInt *y) {
                                                                cout << "call bigAdd... " << endl;
    if (x->sign == y->sign) {
        int sign = x->sign;
        vector<int> sum;
        vector<int>::iterator iterx = x->val.begin();
        vector<int>::iterator itery = y->val.begin();
        int up = 0;
        while (iterx != x->val.end() && itery != y->val.end()) {
            int sum_ = abs(*iterx) + abs(*itery) + up;
            if (sum_ >= 10) {
                sum_ -= 10;
                up = 1;
            }
            else up = 0;
            sum.push_back(sign * sum_);
            iterx++;
            itery++;
        }
        while (iterx != x->val.end()) {
            int sum_ = abs(*iterx) + up;
            if (sum_ >= 10) {
                sum_ -= 10;
                up = 1;
            }
            else up = 0;
            sum.push_back(sign * sum_);
            iterx++;
        }
        while (itery != y->val.end()) {
            int sum_ = abs(*itery) + up;
            if (sum_ >= 10) {
                sum_ -= 10;
                up = 1;
            }
            else up = 0;
            sum.push_back(sign * sum_);
            itery++;
        }
        if (up == 1) 
            sum.push_back(sign);
        BigInt res(sum);
                                                                cout << "val(倒序): ";
                                                                for (int i = 0; i < sum.size(); cout << sum[i], i++);
                                                                cout << endl;
        return res;
    }
    else {
        vector<int> sub;
        vector<int>::iterator iterx = x->val.begin();
        vector<int>::iterator itery = y->val.begin();
        while (iterx != x->val.end() && itery != y->val.end()) {
            sub.push_back(*iterx + *itery);
            iterx++;
            itery++;
        }
        while (iterx != x->val.end()) {
            sub.push_back(*iterx);
            iterx++;
        }
        while (itery != y->val.end()) {
            sub.push_back(*itery);
            itery++;
        }
        while (sub[sub.size() - 1] == 0) {
            sub.pop_back();
        }
        if (sub[sub.size() - 1] < 0) {
            int up = 0;
            for (int i = 0; i < sub.size(); i++) {
                if (sub[i] + up > 0) {
                    sub[i] = sub[i] + up - 10;
                    up = 1;
                }
                else {
                    sub[i] += up;
                    up = 0;
                }
            }
        }
        else {
            int up = 0;
            for (int i = 0; i < sub.size(); i++) {
                if (sub[i] + up < 0) {
                    sub[i] = sub[i] + up + 10;
                    up = -1;
                }
                else {
                    sub[i] += up;
                    up = 0;
                }
            }
        }
        while (sub[sub.size() - 1] == 0) {
            sub.pop_back();
        }
        BigInt res(sub);
                                                                cout << "val(倒序): ";
                                                                for (int i = 0; i < sub.size(); cout << sub[i], i++);
                                                                cout << endl;
        return res;
    }
}


Value operator + (Value x, Value y) {
    if (x.type == INT && y.type == INT) {
        Value sum(INT);
        sum.pInt = new Int(x.pInt->val + y.pInt->val);
        return sum;
    }
    if (x.type == INT && y.type == FLOAT) {
        Value sum(FLOAT);
        sum.pFloat = new Float((float)x.pInt->val + y.pFloat->val);
        return sum;
    }
    if (x.type == INT && y.type == BIGINT) {
        Value sum(BIGINT);
        BigInt x_ = intTObigint(x.pInt);
        BigInt res = bigAdd(&x_, y.pBigInt);
        sum.pBigInt = new BigInt(res);
        return sum;
    }
    if (x.type == FLOAT && y.type == INT) {
        Value sum(FLOAT);
        sum.pFloat = new Float(x.pFloat->val + (float)y.pInt->val);
        return sum;
    }
    if (x.type == FLOAT && y.type == FLOAT) {
        Value sum(FLOAT);
        sum.pFloat = new Float(x.pFloat->val + y.pFloat->val);
        return sum;
    }
    if (x.type == FLOAT && y.type == BIGINT) {
        throw runtime_error("unsupported operation: float +/- bigInt");
    }
    if (x.type == BIGINT && y.type == INT) {
        Value sum(BIGINT);
        BigInt y_ = intTObigint(y.pInt);
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
    memstr.push_back(ini_input);
    memres.push_back(ans);
    result = new Value(ans);
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