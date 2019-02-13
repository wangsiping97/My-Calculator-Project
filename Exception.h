#include <iostream>
using namespace std;

#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception {
public: 
    Exception() {}
    virtual ~Exception() {}
    virtual void printError() {}
};

class TypeError : public Exception {
public: 
    TypeError(string str, int i):str_(str), i_(i){}
    ~TypeError() {}
    virtual void printError(){
        cout << "TypeError: " << i_ << endl;
        cout << str_ << endl;
        for (int j = 0; j < i_; j++) {
            cout <<" ";
        }
        cout << "^" << endl;
    }
    string str_;
    int i_;
};

#endif