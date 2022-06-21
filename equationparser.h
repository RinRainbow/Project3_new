#ifndef EQUATIONPARSER_H
#define EQUATIONPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "qcustomplot.h"

using namespace std;

class EquationParser {

public:
    EquationParser();
    EquationParser(string input, double range = 10, double precision = 10);
    void setString(string input);
    void setRange(double range);
    void setPrecision(double precision);
    void getVector(QVector<double>& x_, QVector<double>& y_);

    QVector<double>xVec, yVec;
    int id;
    bool exception = false;
    string warning = "";
private:
    vector<string> postfix;
    string input;
    double leftRange, rightRange, precision;

    static vector<pair<string, string>> varList;
    void checkInput(const string& input);
    void checkLeftVariable(const string& input);

    bool isOper(char s);
    bool stopVar(char s);
    bool isNum(string input);
    int priority(char op);

    void addVar(string input);
    void replaceVar(string& input);
    void parser(char unknown, const string& input);
    double calculate(double x);

};



#endif // EQUATIONPARSER_H
