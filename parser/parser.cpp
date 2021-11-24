#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "scanner.h"
#include "parser.h"
using namespace std;

static tokenType token;


extern int lineNumber;
extern string lexeme;

void printToken(tokenType _token, ifstream& readFile, ofstream& outFile, bool _expected);

static void syntaxError(string message, ifstream& readFile, ofstream& outFile) {
	outFile << ">>> Syntax error at line " << lineNumber << " : " << message;
}

static bool match(tokenType expected, ifstream& readFile, ofstream& outFile) { //match -> true, error -> false
	if (token == expected) {
		token = getToken(readFile, outFile);
		return true;
	}
	else {
		outFile << ">>> Syntax error at line " << lineNumber << " : " << "Expected Token : ";
		printToken(expected, readFile, outFile,1);
		outFile << " // But unexpected token -> ";
		printToken(token, readFile,outFile,0);
		outFile << " come. \n";
		return false;
	}
}


//declaration_list -> declaration {declaration}
treeNode* declaration_list(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	treeNode* q = declaration(readFile,outFile);

	if (token == ENDFILE) {
		t = q;
	}
	else {
		t = new treeNode("STMT", "DECLARATION_LIST");
		t->child.push_back(q);
		while (token != ENDFILE) {
			t->child.push_back(declaration(readFile,outFile));
		}
	}


	return t;
}

//declaration -> var_declaration | fun-declaration

treeNode* declaration(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	string declaration_type_specifier;
	string declaration_ID_attr;


	declaration_type_specifier = type_specifier(readFile,outFile);
	declaration_ID_attr = lexeme;

	match(ID, readFile, outFile);

	switch (token) {
	case SEMI: // var_declaration -> type_specifier ID ;
	case LSQBR: // var_declaration -> type_specifier ID [NUM];
		t = var_declaration(declaration_type_specifier, declaration_ID_attr, readFile, outFile);
		break;
	case LPR: // fun_declaration -> type_specifier ID ( params ) compound_stmt
		t = fun_declaration(declaration_type_specifier, declaration_ID_attr, readFile, outFile);
		break;
	default:
		syntaxError("unexpected token (declaration) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
	}

	return t;
}

treeNode* var_declaration(string _type_specifier, string _ID_attr, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	
	string var_declaration_type_specifier = _type_specifier;
	string var_declaration_ID_attr = _ID_attr;

	if (_type_specifier == "NULL") {
		var_declaration_type_specifier = type_specifier(readFile, outFile);
	}

	if (_ID_attr == "NULL") {
		var_declaration_ID_attr = lexeme;
		match(ID, readFile, outFile);
	}

	t = new treeNode("STMT", "VAR_DECLAR");
	
	t->addAttr(var_declaration_type_specifier);
	t->addAttr(var_declaration_ID_attr);

	if (token == LSQBR) {
		t->addAttr("ARRAY");
		match(LSQBR, readFile, outFile);
		t->child.push_back(factor("NULL",NULL,readFile,outFile));
		match(RSQBR, readFile, outFile);
	}

	match(SEMI, readFile, outFile);
	
	return t;
}

treeNode* fun_declaration(string _type_specifier, string _ID_attr, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	string fun_declaration_type_specifier = _type_specifier;
	string fun_declaration_ID_attr = _ID_attr;

	if (_type_specifier == "NULL") {
		fun_declaration_type_specifier = type_specifier(readFile, outFile);
	}

	if (_ID_attr == "NULL") {
		fun_declaration_ID_attr = lexeme;
		match(ID, readFile, outFile);
	}

	t = new treeNode("STMT", "FUN_DECLAR");
	t->addAttr(_type_specifier);
	t->addAttr(_ID_attr);
	match(LPR, readFile, outFile);
	t->child.push_back(params(readFile, outFile));
	match(RPR, readFile, outFile);
	t->child.push_back(compound_stmt(readFile, outFile));

	return t;
}

// params -> params_list | void
treeNode* params(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	string param_type;

	param_type = type_specifier(readFile, outFile);

	if (param_type == "VOID" && token == RPR) { // RPR is follow set of params. then, param_type == VOID && token == RPR Means, we choice params -> void
		t = new treeNode("EXP", "PARAMS");
		t->addAttr("VOID");
	}
	else {

		t = new treeNode("EXP", "PARAMS");
		t->child.push_back(params_list(param_type, readFile, outFile));
	}

	return t;
}

treeNode* params_list(string _type, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("EXP", "PARAMS_LIST");

	t->child.push_back(param(_type, readFile, outFile));

	while (token == COMMA) {
		match(COMMA,readFile, outFile);

		t->child.push_back(param("NULL", readFile, outFile));
	}


	return t;
}

treeNode* param(string _type, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("EXP", "PARAM");

	string paramCurrentType = _type;

	if (_type == "NULL") {
		paramCurrentType = type_specifier(readFile, outFile);
	}

	t->addAttr(paramCurrentType);

	t->addAttr(lexeme);
	match(ID, readFile, outFile);

	if (token == LSQBR) {
		match(LSQBR, readFile, outFile);
		match(RSQBR, readFile, outFile);
		t->addAttr("ARRAY");
	}
	else {
		t->addAttr("NOTARRAY");
	}

	return t;
}

treeNode* compound_stmt(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("STMT", "COMPOUND_STMT");

	match(LCUBR,readFile, outFile);
	t->child.push_back(local_declarations(readFile, outFile));
	t->child.push_back(statement_list(readFile, outFile));
	match(RCUBR, readFile, outFile);

	return t;
}

treeNode* local_declarations(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	if ((token != ID) && (token != SEMI) && (token != DIGIT) && (token != LPR) && (token != LCUBR) && (token != IF) && (token != WHILE) && (token != RETURN)) {
		t = new treeNode("STMT", "LOCAL_DECLAR");

		while ((token != ID) && (token != SEMI) && (token != DIGIT) && (token != LPR) && (token != LCUBR) && (token != IF) && (token != WHILE) && (token != RETURN)) {
			//while token is not be statement_list's first, maker var-declarations.
			t->child.push_back(var_declaration("NULL", "NULL", readFile, outFile));
		}
	}
	
	return t;
}

treeNode* statement_list(ifstream & readFile, ofstream & outFile) {
	treeNode* t = NULL;

	if (token != RCUBR) {
		t = new treeNode("STMT", "STMT_LIST");
		while (token != RCUBR) {
			t->child.push_back(statement(readFile, outFile));
		}
	}

	return t;
}

treeNode* statement(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	
	switch (token) {
	case ID:
	case SEMI:
	case DIGIT:
		t = expression_stmt(readFile, outFile);
		break;
	case LCUBR:
		t = compound_stmt(readFile, outFile);
		break;
	case IF:
		t = selection_stmt(readFile, outFile);
		break;
	case WHILE:
		t = iteration_stmt(readFile, outFile);
		break;
	case RETURN:
		t = return_stmt(readFile, outFile);
		break;
	default:
		syntaxError("unexpected token (statement) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
	}

	return t;
}

treeNode* expression_stmt(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	if (token == SEMI) {
		match(SEMI, readFile, outFile);
	}
	else {
		t = expression(readFile, outFile);
		match(SEMI, readFile, outFile);
	}
	

	return t;
}

treeNode* expression(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	string expression_ID;

	treeNode* expression_var = NULL;

	switch (token) {
	case DIGIT:
	case LPR:
		t = simple_expression("NULL", NULL, readFile, outFile);
		break;
	case ID:
		expression_ID = lexeme;
		match(ID, readFile, outFile);

		if (token == LPR) { //this is call case, expression -> simple_expression -> additive_expression - > term -> factor -> call
			t = simple_expression(expression_ID, NULL, readFile, outFile);
		}
		else {
			expression_var = var(expression_ID, readFile, outFile);
			if (token == EQ) { //this is expression -> var = expression
				t = new treeNode("EXP", "EXPRESSION");
				t->child.push_back(expression_var);
				match(EQ, readFile, outFile);
				t->child.push_back(expression(readFile, outFile));
			}
			else{ // this is expression -> simple_expression -> additive_expression - > term -> factor -> var
				t = simple_expression(expression_ID, expression_var, readFile, outFile);
			}
		}

	}

	
	return t;
}

treeNode* var(string _ID, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;

	t = new treeNode("EXP", "VAR");

	string var_id = _ID;

	if (_ID == "NULL") {
		var_id = lexeme;
		match(ID, readFile, outFile);
	}

	t->addAttr(_ID);

	if (token == LSQBR) {
		match(LSQBR,readFile,outFile);
		t->child.push_back(expression(readFile, outFile));
		match(RSQBR, readFile, outFile);
	}


	return t;
}

treeNode* simple_expression(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	
	treeNode* q = NULL;

	q = additive_expression(_ID, _var, readFile, outFile);
	
	if ((token == GE) || (token == GT) || (token == LT) || (token == LE) || (token == EE) || (token == NEQ)) {
		t = new treeNode("EXP", "SIMPLE_EXPRESSION");
		t->child.push_back(q);
		t->addAttr(relop(readFile, outFile));
		t->child.push_back(additive_expression("NULL", NULL, readFile, outFile));
	}
	else {
		t = q;
	}

	return t;
}

treeNode* additive_expression(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	treeNode* q = NULL;

	q = term(_ID, _var, readFile, outFile);

	if ((token == PLUS) || (token == MINUS)) {
		t = new treeNode("EXP", "ADDITIVE_EXPRESSION");
		t->child.push_back(q);
		t->child.push_back(addop(readFile, outFile));
		t->child.push_back(term("NULL", NULL, readFile, outFile));

		while ((token == PLUS) || (token == MINUS)) {
			t->child.push_back(addop(readFile, outFile));
			t->child.push_back(term("NULL", NULL, readFile, outFile));
		}
		
	}
	else {
		t = q;
	}

	return t;
}

treeNode* term(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	treeNode* q = NULL;

	q = factor(_ID, _var, readFile, outFile);

	if ((token == MUL) || (token == DIV)) {
		t = new treeNode("EXP", "TERM");
		t->child.push_back(q);
		t->child.push_back(mulop(readFile, outFile));
		t->child.push_back(factor("NULL", NULL, readFile, outFile));

		while ((token == MUL) || (token == DIV)) {
			t->child.push_back(mulop(readFile, outFile));
			t->child.push_back(factor("NULL", NULL, readFile, outFile));
		}
	}
	else {
		t = q;
	}


	return t;
}

treeNode* factor(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	treeNode* q = NULL;

	string factor_id;


	if (_var != NULL) {
		t = _var;
	}
	else if ((_ID != "NULL") && (_var == NULL)) {
		t = call(_ID, readFile, outFile);
	}
	else if (token == DIGIT) {
		t = new treeNode("EXP", "NUMBER");
		t->addAttr(lexeme);
		match(DIGIT, readFile, outFile);
	}
	else if (token == LPR) {
		t = new treeNode("EXP", "FACTOR");
		t->addAttr("()");
		match(LPR, readFile, outFile);
		t->child.push_back(expression(readFile, outFile));
		match(RPR, readFile, outFile);
	}
	else if (token == ID) {
		factor_id = lexeme;
		match(ID, readFile, outFile);
		if (token == LPR) {
			t = call(factor_id, readFile, outFile);
		}
		else {
			t = var(factor_id, readFile, outFile);
		}
	}

	return t;
}

treeNode* call(string _ID, ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("EXP", "CALL");
	string call_id = _ID;
	if (_ID == "NULL") {
		call_id = lexeme;
		match(ID, readFile, outFile);
	}
	t->addAttr(call_id);
	match(LPR, readFile, outFile);
	t->child.push_back(args(readFile, outFile));
	match(RPR, readFile, outFile);

	return t;
}


treeNode* args(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	
	if (token == RPR) { // args-> epsilon
		// do nothing. (return NULL)
	}
	else {
		t = arg_list(readFile, outFile);
	}

	return t;
}

treeNode* arg_list(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	treeNode* q = NULL;
	
	q = expression(readFile, outFile);

	if (token == COMMA) {
		t = new treeNode("EXP", "ARG_LIST");
		t->child.push_back(q);
		match(COMMA, readFile, outFile);
		t->child.push_back(expression(readFile, outFile));

		while (token == COMMA) {
			match(COMMA, readFile, outFile);
			t->child.push_back(expression(readFile, outFile));
		}
	}
	else {
		t = q;
	}


	return t;
}


treeNode* mulop(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("EXP", "MULOP");
	switch (token) {
	case MUL:
		match(MUL, readFile, outFile);
		t->addAttr("MUL");
		break;
	case DIV:
		match(DIV, readFile, outFile);
		t->addAttr("DIV");
		break;
	default:
		syntaxError("unexpected token (mulop) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
		t->addAttr("ERROR");
	}

	return t;
}

treeNode* addop(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("EXP", "ADDOP");
	switch (token) {
	case PLUS:
		match(PLUS, readFile, outFile);
		t->addAttr("PLUS");
		break;
	case MINUS:
		match(MINUS, readFile, outFile);
		t->addAttr("MINUS");
		break;
	default:
		syntaxError("unexpected token (addop) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
		t->addAttr("ERROR");
	}

	return t;
}


string relop(ifstream& readFile, ofstream& outFile) {

	switch (token) {
	case GE:
		match(GE, readFile, outFile);
		return "GE";
		break;
	case GT:
		match(GT, readFile, outFile);
		return "GT";
		break;
	case LT:
		match(LT, readFile, outFile);
		return "LT";
		break;
	case LE:
		match(LE, readFile, outFile);
		return "LE";
		break;
	case EE:
		match(EE, readFile, outFile);
		return "EE";
		break;
	case NEQ:
		match(NEQ, readFile, outFile);
		return "NEE";
		break;
	default:
		syntaxError("unexpected token (relop) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
		return "ERROR";
	}
}

treeNode* selection_stmt(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("STMT", "SELECTION_STMT");

	match(IF, readFile, outFile);
	match(LPR, readFile, outFile);
	t->child.push_back(expression(readFile, outFile));
	match(RPR, readFile, outFile);
	t->child.push_back(statement(readFile, outFile));
	
	if (token == ELSE) {
		match(ELSE, readFile, outFile);
		t->child.push_back(statement(readFile, outFile));
	}


	return t;
}

treeNode* iteration_stmt(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("STMT", "ITERATION_STMT");

	match(WHILE, readFile, outFile);
	match(LPR, readFile, outFile);
	t->child.push_back(expression(readFile, outFile));
	match(RPR, readFile, outFile);
	t->child.push_back(statement(readFile, outFile));

	return t;
}

treeNode* return_stmt(ifstream& readFile, ofstream& outFile) {
	treeNode* t = NULL;
	t = new treeNode("STMT", "RETURN_STMT");
	match(RETURN, readFile, outFile);
	if (token != SEMI) {
		t->child.push_back(expression(readFile, outFile));
	}
	match(SEMI, readFile, outFile);

	return t;
}



string type_specifier(ifstream& readFile, ofstream& outFile) {
	switch (token) {
	case INT:
		match(INT,readFile,outFile);
		return "INT";
		break;
	case VOID:
		match(VOID, readFile, outFile);
		return "VOID";
		break;
	default :
		syntaxError("unexpected token (type_specifier) -> ", readFile, outFile);
		printToken(token, readFile, outFile,0);
		token = getToken(readFile, outFile);
		return "ERROR";
	}
}

treeNode* parse(ifstream& readFile, ofstream& outFile) {
	treeNode* t;
	token = getToken(readFile, outFile);
	t = declaration_list(readFile, outFile);
	if (token != ENDFILE) {
		syntaxError("Code ends before file\n", readFile, outFile);
	}

	return t;
}

void printTree(treeNode *root, ofstream& outFile, int _depth) {

	if (root == NULL) {
		return;
	}

	for (int i = 0; i < _depth; i++) {
		outFile << "\t";
	}

	outFile << "[" << root->nodeKInd << ":" << root->kind << ']';
	
	for (int i = 0; i < root->attr.size(); i++) {

		if (i == 0) {
			outFile << ", attr -> ";
		}

		if (i == root->attr.size() - 1) {
			outFile << root->attr[i];
		}
		else {
			outFile << root->attr[i] << ',';
		}
	}
	outFile << "\n";

	for (int i = 0; i < root->child.size(); i++) {
		printTree(root->child[i], outFile, _depth +1);
	}
}

void printToken(tokenType _token, ifstream& readFile, ofstream& outFile, bool _expected) {

	switch (_token) {
	case ELSE: //reserved words
	case IF:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		outFile << "reserved word: " << lexeme;
		break;
	case PLUS:
		outFile << "+"; break;
	case MINUS:
		outFile << "-"; break;
	case MUL:
		outFile << "*"; break;
	case DIV:
		outFile << "/"; break;
	case LT:
		outFile << "<"; break;
	case LE:
		outFile << "<="; break;
	case GT:
		outFile << ">"; break;
	case GE:
		outFile << ">="; break;
	case EQ:
		outFile << "="; break;
	case EE:
		outFile << "=="; break;
	case NEQ:
		outFile << "!="; break;
	case SEMI:
		outFile << ";"; break;
	case COMMA:
		outFile << ","; break;
	case LPR:
		outFile << "("; break;
	case RPR:
		outFile << ")"; break;
	case LCUBR:
		outFile << "{"; break;
	case RCUBR:
		outFile << "}"; break;
	case LSQBR:
		outFile << "-"; break;
	case RSQBR:
		outFile << "-"; break;
	case ENDFILE:
		outFile << "ENDFILE"; break;
	case ERR:
		outFile << "ERROR: " << lexeme; break;
	case ID:
		if (_expected) {
			outFile << "ID"; break;
		}
		else {
			outFile << "ID, name= " << lexeme;
		}
		break;
	case DIGIT:
		if (_expected) {
			outFile << "DIGIT";
		}
		else {
			outFile << "DIGIT, name= " << lexeme;
		}
		break;
	default: // this should never happened!
		outFile << "Unknown Token: " << lexeme << " " << token ;
	}
}
