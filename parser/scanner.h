#include <string>
#include <fstream>
using namespace std;


typedef enum {
	START, IN_ID, IN_DIGIT, IN_ONE_SYMBOL, IN_LT, IN_LE, IN_GT, IN_GE, IN_EQ, IN_EE, IN_WOW, IN_WE, IN_SEP, IN_COMMENT1, IN_COMMENT2, DONE, ERROR, ID_DIGIT_TOKEN_ERROR
}ScannerState;

typedef enum
/* book-keeping tokens */
{
	ENDFILE, ERR,
	/* reserved words */
	ELSE, IF, INT, RETURN, VOID, WHILE,
	/* multicharacter tokens */
	ID, DIGIT,
	/* special symbols */
	PLUS, MINUS, MUL, DIV, LT, LE, GT, GE, EQ, EE, NEQ, SEMI, COMMA, LPR, RPR, LCUBR, RCUBR, LSQBR, RSQBR
} tokenType;

tokenType check_keywords(string temp);
tokenType getToken(ifstream &readFile, ofstream &outFile);