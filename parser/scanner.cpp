#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "scanner.h"
using namespace std;

int lineNumber = 1;
string lexeme;


tokenType check_keywords(string temp) {
	if (temp == "else") {
		return ELSE;
	}
	else if (temp == "if") {
		return IF;
	}
	else if (temp == "return") {
		return RETURN;
	}
	else if (temp == "void") {
		return VOID;
	}
	else if (temp == "while") {
		return WHILE;
	}
	else if (temp == "int") {
		return INT;
	}else{
		return ID;
	}
}


int buffer_Iterator = 0;
string buffer;

ScannerState scannerState = START;


tokenType getToken(ifstream &readFile, ofstream &outFile) {
	scannerState = START;
	tokenType returnData = ERR;

	while (1) { //read file for end

		if (buffer_Iterator == buffer.size()) {

			if (readFile.peek() == EOF) {
				return ENDFILE;
			}
			else {
				getline(readFile, buffer);
				// outFile << lineNumber << ": " << buffer << endl; //print current line

				buffer.push_back('\n');
				buffer_Iterator = 0;
			}
		}

		//if, example sentece is "Hello, world!"
		//buffer is
		// h/e/l/l/o/ /w/o/r/l/d/!/\n/


		while (buffer_Iterator != buffer.size()) {

			char nextChar;

			nextChar = buffer[buffer_Iterator];

			if (scannerState == START) {
				//When Scanner in START state, clear lexeme
				lexeme.clear();
				

				if (nextChar == ' ' || nextChar == '\t' || nextChar == '\n' || nextChar == '\r') {
					//When nextChar is white space (space, tab, newline)

					scannerState = START;
					buffer_Iterator++;
				}
				else if ((nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= 'a' && nextChar <= 'z')) {
					//When nextChar is letter
					scannerState = IN_ID;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = ID;
				}
				else if (nextChar >= '0' && nextChar <= '9') {
					//When nextChar is digit
					scannerState = IN_DIGIT;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = DIGIT;
				}
				else if (nextChar == '+' || nextChar == '-' || nextChar == '*' || nextChar == ';' || nextChar == ',' || nextChar == '(' || nextChar == ')'
					|| nextChar == '[' || nextChar == ']' || nextChar == '{' || nextChar == '}') {
					//When nextChar is one-charactor-symbol
					scannerState = IN_ONE_SYMBOL;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					switch (nextChar) {
					case '+':
						returnData = PLUS;
						break;
					case '-':
						returnData = MINUS;
						break;
					case '*':
						returnData = MUL;
						break;
					case ';':
						returnData = SEMI;
						break;
					case ',':
						returnData = COMMA;
						break;
					case '(':
						returnData = LPR;
						break;
					case ')':
						returnData = RPR;
						break;
					case '[':
						returnData = LSQBR;
						break;
					case ']':
						returnData = RSQBR;
						break;
					case '{':
						returnData = LCUBR;
						break;
					case '}':
						returnData = RCUBR;
						break;
					}
				}
				else if (nextChar == '<') {
					//When nextChar is < (Less Then, LT)
					scannerState = IN_LT;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = LT;
				}
				else if (nextChar == '>') {
					//When nextChar is > (Great Then, GT)
					scannerState = IN_GT;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = GT;
				}
				else if (nextChar == '=') {
					//When nextchar is = (EQual)
					scannerState = IN_EQ;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = EQ;
				}
				else if (nextChar == '!') {
					//When nextchar is ! (WOW symbol :P )
					scannerState = IN_WOW;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
				}
				else if (nextChar == '/') {
					//when nextchar is / (SEParate)
					scannerState = IN_SEP;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = DIV;
				}
				else {
					scannerState = ERROR;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = ERR;
				}

			}
			else if (scannerState == IN_ID) {
				//When Scanner in IN_ID state

				if ((nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= 'a' && nextChar <= 'z')) {
					//when nextchar is letter
					scannerState = IN_ID;
					lexeme.push_back(nextChar);
					if (check_keywords(lexeme) != ID) {
						//when our token is keyword
						scannerState = DONE;
						returnData = check_keywords(lexeme);
						// outFile << "\t" << lineNumber << ": reserved word: ";
					}
					buffer_Iterator++;
				}
				else if (nextChar >= '0' && nextChar <= '9') {
					//when nextchar is DIGIT
					scannerState = ID_DIGIT_TOKEN_ERROR;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = ERR;
				}
				else {
					//when nextchar is NOT letter
					scannerState = DONE;
					returnData = ID;
					// outFile << "\t" << lineNumber << ": ID, name = ";
				}
			}
			else if (scannerState == IN_DIGIT) {
				//When Scanner in IN_DIGIT state
				if (nextChar >= '0' && nextChar <= '9') {
					//when nextchar is digit
					scannerState = IN_DIGIT;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = DIGIT;
				}
				else if ((nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= 'a' && nextChar <= 'z')) {
					//when nextchar is letter
					scannerState = ID_DIGIT_TOKEN_ERROR;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = ERR;
				}
				else {
					//when nextchar is NOT digit
					scannerState = DONE;
					returnData = DIGIT;
					//outFile << "\t" << lineNumber << ": NUM, val = ";
				}
			}
			else if (scannerState == IN_ONE_SYMBOL) {
				//When Scanner is IN_ONE_SYMBOL state
				//outFile << "\t" << lineNumber << ": ";
				scannerState = DONE;
			}
			else if (scannerState == IN_LT) {
				//When Scanner is IN_LT(<) state
				if (nextChar == '=') {
					//When nextchar is = : go to LE(<=) state
					scannerState = IN_LE;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = LE;
				}
				else {
					//token is end ("<")
					scannerState = DONE;
					returnData = LT;
				}
			}
			else if (scannerState == IN_GT) {
				//when Scanner is IN_GT(>) state
				if (nextChar == '=') {
					//When nextchar is = : go to GE(>=) state
					scannerState = IN_GE;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = GE;
				}
				else {
					//token is end (">")
					scannerState = DONE;
					returnData = GT;
				}
			}
			else if (scannerState == IN_EQ) {
				//When Scanner is IN_EQ(=) state
				if (nextChar == '=') {
					//When nextchar is = : go to EE(==) state
					scannerState = IN_EE;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = EE;
				}
				else {
					//token is end ("=")
					// outFile << "\t" << lineNumber << ": ";
					scannerState = DONE;
					returnData = EQ;
				}
			}
			else if (scannerState == IN_EE) {
				//When Scanner is IN_EE(==) state
				// outFile << "\t" << lineNumber << ": ";
				scannerState = DONE;
				returnData = EE;
			}
			else if (scannerState == IN_WOW) {
				//When Scanner is IN_WOW(!) state
				if (nextChar == '=') {
					//When nextchar is = : go to WE(!=) state
					scannerState = IN_WE;
					lexeme.push_back(nextChar);
					buffer_Iterator++;
					returnData = NEQ;
				}
				else {
					//ERROR!! 
					scannerState = ERROR;
					returnData = ERR;
				}
			}
			else if (scannerState == IN_WE) {
				//When Scanner is IN_WE(!=) state
				// outFile << "\t" << lineNumber << ": ";
				scannerState = DONE;
				returnData = NEQ;
			}
			else if (scannerState == IN_SEP) {
				//When Scanner is IN_SEP state
				if (nextChar == '*') {
					//When nextchar is * : go to IN_COMMENT1(/*) state
					scannerState = IN_COMMENT1;
					lexeme.clear();
					buffer_Iterator++;
				}
				else {
					//when nextchar is NOT * : go to DONE state token is end ("/")
					// outFile << "\t" << lineNumber << ": ";
					scannerState = DONE;
					returnData = DIV;
				}
			}
			else if (scannerState == IN_COMMENT1) {
				if (nextChar == '*') {
					scannerState = IN_COMMENT2;
					buffer_Iterator++;
				}
				else {
					scannerState = IN_COMMENT1;
					buffer_Iterator++;
				}
			}
			else if (scannerState == IN_COMMENT2) {
				if (nextChar == '/') {
					scannerState = START;
					buffer_Iterator++;
				}
				else if (nextChar == '*') {
					scannerState = IN_COMMENT2;
					buffer_Iterator++;
				}
				else {
					scannerState = IN_COMMENT1;
					buffer_Iterator++;
				}
			}
			else if (scannerState == DONE) {
				// outFile << lexeme << endl;

				scannerState = START;
				return returnData;
			}
			else if (scannerState == ERROR) {
				// outFile << "\t" << lineNumber << ": ERROR: " << lexeme << endl;
				lexeme.clear();

				scannerState = START;
				return ERR;
			}
			else if (scannerState == ID_DIGIT_TOKEN_ERROR) {
				if (!(((nextChar >= 'A' && nextChar <= 'Z') || (nextChar >= 'a' && nextChar <= 'z')) || (nextChar >= '0' && nextChar <= '9'))) {
					//nextChar is not letter and digit
					//outFile << "\t" << lineNumber << ": ERROR: " << lexeme << endl;

					scannerState = START;
					return ERR;
				}
				else {
					lexeme.push_back(nextChar);
					buffer_Iterator++;
				}
			}
		}

		lineNumber++;
	}
}