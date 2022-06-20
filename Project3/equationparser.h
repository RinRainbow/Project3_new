#ifndef EQUATIONPARSER_H
#define EQUATIONPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <exception>
#include <sstream>

using namespace std;

class EquationParser
{

public:
    EquationParser();
    EquationParser(string input, double range = 10, double precision = 10);
    void setString(string input);
    void setRange(double range);
    void setPrecision(double precision);
    void getVector(vector<double> &x_, vector<double> &y_);


private:
    //QVector<double>xVec, yVec;
    vector<double> xVec, yVec;
    vector<string> postfix;
    vector<pair<string, string>> varList;
    string input;
    double leftRange, rightRange, precision;

    int Error;
    enum ERROR_CODE {
        No_Error = 0,
        Invaild_Input = 1,
        Invaild_Variable = 2,
        Variable_Already_Exists = 3,
        Undefined_Variable = 4
    };

    void checkInput(const string &input);
    void checkLeftVariable(const string &input);

    bool isOper(char s);
    bool stopVar(char s);
    bool isNum(string input);
    int priority(char op);

    void addVar(string input);
    void replaceVar(string &input);
    void parser(const string& input);
    double calculate(double x);

};



#endif // EQUATIONPARSER_H
