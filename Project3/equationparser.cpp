#include "equationparser.h"

vector<pair<string, string>> EquationParser::varList = {};

EquationParser::EquationParser() {
	this->input = "";
	this->leftRange = -100;
	this->rightRange = 100;
	this->precision = 0.001;
}

EquationParser::EquationParser(string input, double range, double precision) {
	this->leftRange = -range;
	this->rightRange = range;
	this->precision = precision;
	this->input = input;
}

void EquationParser::setString(string input) {
	this->input = input;
	xVec.clear();
	yVec.clear();

	// check input 
	checkInput(input);
	checkLeftVariable(input);

	// input processing
	// remove space
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

	// left is y
	double Max = 100000000;
	if (input[0] == 'y') {
		size_t found = input.find("=");
		string rightStr = input.substr(found + 1);
		replaceVar(rightStr);
		parser('x', rightStr);

		double y;
		for (double x = leftRange; x <= rightRange; x += precision) {
			y = calculate(x);
			xVec.push_back(x);
			yVec.push_back(y);
			/*
			if (yVec.size() > 2) {
				double preY = yVec[yVec.size() - 2],
					curY = yVec[yVec.size() - 1];

				if ((curY - preY) > 100) {
					if (curY > 0) {
						xVec.push_back(x);
						xVec.push_back(std::numeric_limits<double>::quiet_NaN());
						yVec.push_back(y + Max);
						yVec.push_back(std::numeric_limits<double>::quiet_NaN());
					} else {
						xVec.insert(xVec.size() - 2, std::numeric_limits<double>::quiet_NaN());
						xVec.insert(xVec.size() - 2, x);
						yVec.insert(yVec.size() - 2, std::numeric_limits<double>::quiet_NaN());
						yVec.insert(yVec.size() - 2, y - Max);
					}
				}
			}
			*/
		}
	}
	// left is x
	else if (input[0] == 'x') {
		size_t found = input.find("=");
		string rightStr = input.substr(found + 1);
		replaceVar(rightStr);
		parser('y', rightStr);

		double x;
		double precisionTmp = precision;
		double Max = 0;
		for (double y = leftRange; y <= rightRange; y += precisionTmp) {
			x = calculate(y);
			xVec.push_back(x);
			yVec.push_back(y);
			/*
			if (xVec.size() > 2) {
				double preX = xVec[xVec.size() - 2],
					curX = xVec[xVec.size() - 1];

				if (abs(curX - preX) > 40) {
					xVec.push_back(x + Max);
					yVec.push_back(y);
				}
			}
			*/
		}
	}
	// Is variable
	else {
		addVar(input);
	}
}

void EquationParser::checkInput(const string& input) {
	string vaildStr = "0123456789()+-*/=!.^abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	int bracket = 0;
	int leftBracketIdx = -1, rightBracketIdx = -1;

	// check number of equal sign
	size_t countEqual = count(input.begin(), input.end(), '=');
	if (countEqual != 1)
		throw exception("Invalid input about '='");

	for (int i = 0; i < input.size(); i++) {
		char c = input[i];

		// check if input exists invaild character
		size_t found = vaildStr.find(c);
		if (found == string::npos)
			throw exception("Invalid input about illegal character");

		// check if the parentheses brackets valid
		if (c == '(') {
			bracket++;
			leftBracketIdx = i;
		} else if (c == ')') {
			bracket--;
			rightBracketIdx = i;
			if (leftBracketIdx + 1 == rightBracketIdx)
				throw exception("Parenthese bracket is empty");
		}
		if (bracket < 0)
			throw exception("Imcomplete parentheses brackets");
	}

	if (bracket != 0) {
		throw exception("Imcomplete parentheses brackets");
	}

}

void EquationParser::checkLeftVariable(const string& input) {
	size_t foundEqualIdx = input.find('=');
	if (foundEqualIdx == string::npos)
		throw exception("Invalid input about '='");
	for (int i = 0; i < foundEqualIdx; i++) {
		if (isOper(input[i]))
			throw exception("Invalid input about left equation");
	}
}

void EquationParser::setRange(double range) {
	leftRange = -range;
	rightRange = range;
}

void EquationParser::setPrecision(double precision) {
	this->precision = precision;
}

void EquationParser::getVector(QVector<double>& x_, QVector<double>& y_) {
	xVec.clear();
	yVec.clear();
	// check input 
	string input = this->input;
	checkInput(input);
	checkLeftVariable(input);

	// input processing
	// remove space
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

	// left is y
	if (input[0] == 'y') {
		size_t found = input.find("=");
		string rightStr = input.substr(found + 1);
		replaceVar(rightStr);
		parser('x', rightStr);

		double y;
		for (double x = leftRange; x <= rightRange; x += precision) {
			y = calculate(x);
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	// left is x
	else if (input[0] == 'x') {
		size_t found = input.find("=");
		string rightStr = input.substr(found + 1);
		replaceVar(rightStr);
		parser('y', rightStr);

		double x;
		for (double y = leftRange; y <= rightRange; y += precision) {
			x = calculate(y);
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	// Is variable
	else {
		addVar(input);
	}
	x_ = xVec;
	y_ = yVec;
}

bool EquationParser::isOper(char s) {
	string operStr = "+-*/=!^#~";
	size_t found = operStr.find(s);
	if (found == string::npos) {
		return false;
	}
	return true;
}

bool EquationParser::stopVar(char s) {
	string operStr = "+-*/=!^()";
	size_t found = operStr.find(s);
	if (found == string::npos) {
		return false;
	}
	return true;
}

bool EquationParser::isNum(string input) {
	for (auto c : input) {
		if ((c < '0' || c > '9') && c != '.') {
			return false;
		}
	}
	return true;
}

void EquationParser::addVar(string input) {
	const string reservedWord[5] = { "x", "y", "sin", "cos", "tan" };
	string var;
	size_t foundEqual = input.find('=');

	var = input.substr(0, foundEqual);
	string num = input.substr(foundEqual + 1);
	string varTmp = "";

	for (int i = 0; i < num.size(); i++) {
		bool isAlphabet =
			(num[i] <= 'z' && num[i] >= 'a') ||
			(num[i] <= 'Z' && num[i] >= 'A');

		if (isAlphabet) {
			varTmp += num[i];
		} else {
			if (varTmp == "")
				continue;

			bool isResevedW = false;
			for (auto r : reservedWord) {
				if (varTmp == r) {
					isResevedW = true;
					break;
				}
			}
			if (isResevedW)
				continue;

			if (varList.size() == 0) {
				throw exception("Exist undefined variable");
			}
			for (auto varPair : varList) {
				size_t foundVar = varTmp.find(varPair.first);
				if (foundVar == string::npos) {
					throw exception("Exist undefined variable");
				} else {
					num.erase(foundVar, varPair.first.size());
					num.insert(foundVar, "(" + varPair.second + ")");
				}
			}
			varTmp = "";
		}
	}

	bool exist = 0;
	for (auto& v : varList) {
		if (v.first == var) {
			exist = 1;
			throw exception("Variable already exist");
		}
	}
	if (!exist) {
		varList.push_back(make_pair(var, num));
	}
}

void EquationParser::replaceVar(string& input) {
	const string reservedWord[5] = { "x", "y", "sin", "cos", "tan" };
	string varTmp = "";
	int varIdx = -1;
	bool isVar = false; // if encouter alphabet, then is a variable

	for (int i = 0; i < input.size(); i++) {
		if (input[i] <= 'z' && input[i] >= 'a' ||
			input[i] <= 'Z' && input[i] >= 'A') {
			isVar = true;
			varIdx = i;
			varTmp += input[i];

		} else if (isVar && (input[i] <= '9' && input[i] >= '0')) {
			varTmp += input[i];
		}
		if (stopVar(input[i]) || i == input.size() - 1) {
			if (!isVar)
				continue;


			bool isResevedW = false;
			for (auto r : reservedWord) {
				if (varTmp == r) {
					isResevedW = true;
					break;
				}
			}
			if (!isResevedW) {
				bool foundVar = false;
				for (auto v : varList) {
					if (varTmp == v.first) {
						foundVar = true;
						input.erase(varIdx, varTmp.size());
						input.insert(varIdx, "(" + v.second + ")");
						break;
					}
				}
				if (!foundVar) {
					throw exception("Undefined variable");
				}
			}

			varTmp = "";
			isVar = false;
		}
	}
}

int EquationParser::priority(char op) {
	switch (op) {
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case '^': return 3;
	case '~': case '#': return 4;
	case 's': case 'c': case 't': return 5;

	default:            return 0;
	}
}

void EquationParser::parser(char unknown, const string& input) {
	postfix.clear();
	stack<char> temp;
	bool firstNum = true;
	bool preNum = false;
	bool preRightBracket = false;
	int sPtr = 0, len = 0;
	string str = "";

	// sin, cos, tan
	int funcAmount = 3;
	string function[3] = { "sin(", "cos(", "tan(" };

	for (int i = 0; i < input.size(); i++) {
		char c = input[i];
		if (c == ' ') continue;
		if (c == unknown) {
			string tmp(1, unknown);
			postfix.push_back(tmp);
			preNum = true;
			continue;
		}

		// Parse number
		if ((c >= '0' && c <= '9') || c == '.') {
			if (firstNum) {
				postfix.push_back(str);
				sPtr = i;
			}
			firstNum = false;
			preNum = true;
			preRightBracket = false;
			postfix[postfix.size() - 1].push_back(c);
			continue;
		} else {
			firstNum = true;
			str = "";
		}

		// Parse the sin, cos, and tan
		if (c <= 'z' && c >= 'a') {
			bool isFunc = false;
			for (int j = 0; j < funcAmount; j++) {
				for (int s = 0; s < function[j].size(); s++) {
					if (input[i + s] != function[j][s]) {
						isFunc = false;
						break;
					}
					isFunc = true;
				}
				if (isFunc) {
					c = function[j][0];
					i += function[j].size() - 2;
					break;
				}
			}
			if (!isFunc)
				throw exception("function error");
		}

		// Parse operator
		if (priority(c) > 0) {
			if (!preNum) {
				if (c == '-')
					c = '~';
				else if (c == '+')
					c = '#';
				else if (!preRightBracket && !(c <= 'z' && c >= 'a'))
					throw exception("Operator error");
			}
			if (!temp.empty() && temp.top() != '(') {
				if (priority(temp.top()) >= priority(c)) {
					do {
						string tmp(1, temp.top());
						postfix.push_back(tmp);
						temp.pop();
					} while (!temp.empty() && priority(temp.top()) >= priority(c));
					temp.push(c);
					preNum = false;
				} else {
					temp.push(c);
					preNum = false;
				}
			} else {
				temp.push(c);
			}
			preNum = false;
			preRightBracket = false;
		}

		if (c == '(') {
			temp.push(c);
			preNum = false;
			preRightBracket = false;
		} else if (c == ')') {
			while (!temp.empty()) {
				if (temp.top() == '(') {
					temp.pop();
					break;
				} else {
					string tmp(1, temp.top());
					postfix.push_back(tmp);
					temp.pop();
				}
			}
			preRightBracket = true;
		}
	}
	while (!temp.empty()) {
		string tmp(1, temp.top());
		postfix.push_back(tmp);
		temp.pop();
	}
}

double EquationParser::calculate(double x) {
	stack<double> numStack;
	double num;

	for (int i = 0; i < postfix.size(); i++) {
		string str = postfix[i];
		if (str == "x" || str == "y") {
			numStack.push(x);
		} else if (isNum(str)) {
			std::stringstream ss;
			ss << str;
			ss >> num;
			numStack.push(num);
		} else if (str == "+") {
			num = numStack.top();
			numStack.pop();
			num = num + numStack.top();
			numStack.pop();
			numStack.push(num);
		} else if (str == "-") {
			num = numStack.top();
			numStack.pop();
			num = numStack.top() - num;
			numStack.pop();
			numStack.push(num);
		} else if (str == "*") {
			num = numStack.top();
			numStack.pop();
			num = numStack.top() * num;
			numStack.pop();
			numStack.push(num);
		} else if (str == "/") {
			num = numStack.top();
			numStack.pop();

			if (num == 0) {
				throw exception("Divide by zero");
				break;
			}
			num = numStack.top() / num;
			numStack.pop();
			numStack.push(num);
		} else if (str == "^") {
			num = numStack.top();
			numStack.pop();
			num = pow(numStack.top(), num);
			numStack.pop();
			numStack.push(num);
		} else if (str == "~") {
			num = -numStack.top();
			numStack.pop();
			numStack.push(num);
		} else if (str == "#") {
		} else if (str == "s") {
			num = sin(numStack.top());
			numStack.pop();
			numStack.push(num);
		} else if (str == "c") {
			num = cos(numStack.top());
			numStack.pop();
			numStack.push(num);
		} else if (str == "t") {
			num = tan(numStack.top());
			numStack.pop();
			numStack.push(num);
		}
	}

	return numStack.top();
}




