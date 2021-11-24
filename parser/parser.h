#include <vector>
#include <string>
#include <fstream>
using namespace std;


class treeNode {
public:
	vector<treeNode*> child;
	treeNode* sibling;
	int lineNo;
	string nodeKInd; // STMT , EXP
	string kind;
	//STMT case :VAR_ASSIGN, FUNC_ASSIGN, 
	//EXP case : NUM, PARAMS
	vector<string> attr;

	treeNode(string _nodeKind, string _kind) {
		this->sibling = NULL;
		this->nodeKInd = _nodeKind;
		this->kind = _kind;
	}

	void addAttr(string _attr) {
		this->attr.push_back(_attr);
		return;
	}
};

static treeNode* declaration_list(ifstream& readFile, ofstream& outFile);
static treeNode* declaration(ifstream& readFile, ofstream& outFile);
static treeNode* params(ifstream& readFile, ofstream& outFile);
static string type_specifier(ifstream& readFile, ofstream& outFile);
static treeNode* addop(ifstream& readFile, ofstream& outFile);
static treeNode* additive_expression(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile);
static treeNode* args(ifstream& readFile, ofstream& outFile);
static treeNode* arg_list(ifstream& readFile, ofstream& outFile);
static treeNode* call(string _ID, ifstream& readFile, ofstream& outFile);
static treeNode* return_stmt(ifstream& readFile, ofstream& outFile);
static treeNode* iteration_stmt(ifstream& readFile, ofstream& outFile);
static string relop(ifstream& readFile, ofstream& outFile);
static treeNode* compound_stmt(ifstream& readFile, ofstream& outFile);
static treeNode* expression(ifstream& readFile, ofstream& outFile);
static treeNode* expression_stmt(ifstream& readFile, ofstream& outFile);
static treeNode* factor(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile);
static treeNode* local_declarations(ifstream& readFile, ofstream& outFile);
static treeNode* fun_declaration(string _type_specifier, string _ID_attr, ifstream& readFile, ofstream& outFile);
static treeNode* mulop(ifstream& readFile, ofstream& outFile);
static treeNode* param(string _type, ifstream& readFile, ofstream& outFile);
static treeNode* params_list(string _type, ifstream& readFile, ofstream& outFile);
static treeNode* simple_expression(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile);
static treeNode* term(string _ID, treeNode* _var, ifstream& readFile, ofstream& outFile);
static treeNode* var(string _ID, ifstream& readFile, ofstream& outFile);
static treeNode* selection_stmt(ifstream& readFile, ofstream& outFile);
static treeNode* statement(ifstream& readFile, ofstream& outFile);
static treeNode* statement_list(ifstream& readFile, ofstream& outFile);
static treeNode* var_declaration(string _type_specifier, string _ID_attr, ifstream& readFile, ofstream& outFile);

treeNode* parse(ifstream& readFile, ofstream& outFile);
void printTree(treeNode* root, ofstream& outFile, int _depth);