#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "scanner.h"
#include "parser.h"
using namespace std;

extern int lineNumber;
extern string lexeme;

int main(int argc, char* argv[]) {


	if (argc != 3) {
		cout << "usage: filename inputFile outputFile" << endl;
		cout << argc << endl;
		return 1;
	}
	else {
		string input_fileName = argv[1];
		ifstream readFile(input_fileName);

		string output_fileName = argv[2];
		ofstream outFile(output_fileName);

		treeNode* p = parse(readFile, outFile);

		printTree(p, outFile, 0);

		readFile.close();
		outFile.close();
	}
}