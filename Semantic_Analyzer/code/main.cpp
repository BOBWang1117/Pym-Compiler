/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"analyzer.h"
#include<vector>

using namespace std;

int main(int argc, char ** argv) {

	//scanner
	string fileName="";
	if (argc == 2) {
		fileName= argv[1];
	}
	bool isError = false;
	TokenList* tokenList=scanner(fileName,&isError);

	//no error in scanner
	if (tokenList!=NULL && tokenList->firstNode != NULL&&(!isError)) {
		//parser
		ParserInfo* parserInfo = new ParserInfo(tokenList->firstNode);
		Parser* p = new Parser(parserInfo);
		TreeNode* firstTreeNode = p->parse();
		bool isErrorNd = false;
		printTree(firstTreeNode,&isErrorNd);

		//semantic analyzer
		if (!isErrorNd) {
			Analyzer* a = new Analyzer(firstTreeNode);
			SymbolTable* top= a->start();
			if (top == NULL) {
				cout <<endl<< "Error found by in pre-tranversal, symbol table construction error"<<endl;
			}
			else {
				cout << endl << "The Symbol Table looks like:" << endl;
				top->toString(0);
			}
		}
		else {
			cout << endl << "Error found in parser, stop" << endl;
		}
	}


	return 0;
}
