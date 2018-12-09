#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>;
#include <ctype.h>
#include <map>

using namespace std;

string str, buffer, actualToken, previousToken, actualTokenOfAction, previousTokenOfAction = "";
int actualResult, previousResult;
stack<string> tokens;
int previousPriority, actualPriority;
map<string, int> results = {
};
map<string, int> tokensPriority = {
	{"*",1},
	{"/",1},
	{"%",1},
	{"+",2},
	{"-",2},
	{"<",3},
	{">",3},
	{"<=",3},
	{">=",3},
	{"==",4},
	{"!=",4},
	{"&&", 5},
	{"||", 6}
};

int calculateExpression(int firstToken, int secondToken, string tokenOfAction)
{
	if (tokenOfAction == "+")	return firstToken + secondToken;
	else if (tokenOfAction == "-")	return firstToken - secondToken;
	else if (tokenOfAction == "/")	return firstToken / secondToken;
	else if (tokenOfAction == "*")	return firstToken * secondToken;
	else if (tokenOfAction == "%")	return firstToken % secondToken;
	else if (tokenOfAction == "<")	return firstToken < secondToken;
	else if (tokenOfAction == ">")	return firstToken > secondToken;
	else if (tokenOfAction == "<=")	return firstToken <= secondToken;
	else if (tokenOfAction == ">=")	return firstToken >= secondToken;
	else if (tokenOfAction == "==")	return firstToken == secondToken;
	else if (tokenOfAction == "!=")	return firstToken != secondToken;
	else if (tokenOfAction == "&&")	return firstToken && secondToken;
	else if (tokenOfAction == "||")	return firstToken || secondToken;
}

void parser(string str, int &i)
{
	string   previousToken, previousTokenOfAction = "";
	if (str[i] != '(')
	{
		i = str.find('(');
		i++;
	}
	else
		i++;
	for (i = i; i < str.length(); i++) {
		bool pairsToAction =
			!actualTokenOfAction.empty() &&
			!previousToken.empty() &&
			!previousTokenOfAction.empty();
		bool theEnd = str[i] == ')' &&
			!previousTokenOfAction.empty() &&
			!previousToken.empty() &&
			!actualToken.empty();
		if (str[i] == '(' && !pairsToAction)
		{
			if (!previousToken.empty() && !previousTokenOfAction.empty())
			{
				tokens.push(previousToken);
				tokens.push(previousTokenOfAction);
			}
			else
			{
				tokens.push(actualToken);
				tokens.push(actualTokenOfAction);
				previousToken = actualToken;
				actualToken.erase();
				previousTokenOfAction = actualTokenOfAction;
				actualTokenOfAction.erase();
				previousPriority = actualPriority;
				actualPriority = NULL;
			}
			tokens.push("(");


			parser(str, i);
		}
		if ((actualToken.empty() || actualTokenOfAction.empty()) && !theEnd) {
			while (isalnum(str[i])) {
				actualToken += str[i];
				i++;
			}
			while (str[i] != ')' && str[i] != '('  && ispunct(str[i])) {
				actualTokenOfAction += str[i];
				i++;
			}
			if (!actualTokenOfAction.empty())
				actualPriority = tokensPriority.find(actualTokenOfAction)->second;
			if (str[i] != ' ')
			{
				i--;
			}
		}
		else if (pairsToAction || theEnd) {
			if (pairsToAction)
			{
				if (str[i] != ' ') {
					previousPriority = tokensPriority.find(previousTokenOfAction)->second;
				}
				if (str[i] != ' ') {
					actualPriority = tokensPriority.find(actualTokenOfAction)->second;
				}
			}
			if (str[i] == ')'  || (previousPriority <= actualPriority) )
			{
				if (sscanf_s(previousToken.c_str(), "%*u%*c") == 0 && sscanf_s(actualToken.c_str(), "%*u%*c") == 0)
					actualResult = calculateExpression(results.find(previousToken)->second, results.find(actualToken)->second, previousTokenOfAction);
				else if (sscanf_s(previousToken.c_str(), "%*u%*c") == 0)
					actualResult = calculateExpression(results.find(previousToken)->second, stoi(actualToken), previousTokenOfAction);
				else if (sscanf_s(actualToken.c_str(), "%*u%*c") == 0)
					actualResult = calculateExpression(stoi(previousToken), results.find(actualToken)->second, previousTokenOfAction);
				else
					actualResult = calculateExpression(stoi(previousToken), stoi(actualToken), previousTokenOfAction);

				buffer = previousToken + previousTokenOfAction + actualToken;
				cout << buffer << " " << actualResult << endl;
				actualToken = buffer;
				results.insert(pair<string, int>(buffer, actualResult));
				if (theEnd)
				{
					if (!tokens.empty() && tokens.top() == "(")
					{
						tokens.pop();
						tokens.pop();
						tokens.pop();
						actualToken = "(" + actualToken + ")";
						results.insert(pair<string, int>(actualToken, actualResult));
						i++;
						return;
					}
					else if (!tokens.empty()) {
						previousTokenOfAction = tokens.top();
						previousPriority = tokensPriority.find(previousTokenOfAction)->second;
						tokens.pop();
						previousToken = tokens.top();
						tokens.pop();
					}
					else {
						previousToken.erase();
						previousTokenOfAction.erase();
						if (str[i] == ')')
						{
							cout << str << "\n\n";
							return;
						}
					}
				}
				else
				{
					previousToken = actualToken;
					actualToken.erase();
					previousTokenOfAction = actualTokenOfAction;
					actualTokenOfAction.erase();
					previousPriority = actualPriority;
					actualPriority = NULL;
				}
				i--;
			}
			else if (previousPriority > actualPriority) {
				tokens.push(previousToken);
				tokens.push(previousTokenOfAction);
				previousToken = actualToken;
				actualToken.erase();
				previousTokenOfAction = actualTokenOfAction;
				actualTokenOfAction.erase();
				previousPriority = actualPriority;
				actualPriority = NULL;
				i--;
			}
		}
		else if (!actualToken.empty() && !actualTokenOfAction.empty()) {
			previousToken = actualToken;
			actualToken.erase();
			previousTokenOfAction = actualTokenOfAction;
			previousPriority = actualPriority;
			actualTokenOfAction.erase();
			actualPriority = NULL;
			i--;
		}
	}
}

void main()
{


	int count = 1;
	ifstream fin("example.txt");

	while (getline(fin, str)) {
		cout << "Expression: " << count << endl;
		actualToken.erase();
		actualTokenOfAction.erase();
		int i = 0;
		parser(str, i);
		count++;


	}
	//cout << calculateExpression(1, 2, "+");
	fin.close();

}