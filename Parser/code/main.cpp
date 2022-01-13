/* homework 3. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"parse.h"
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

	if (tokenList->firstNode != NULL&&(!isError)) {
		ParserInfo* parserInfo = new ParserInfo(tokenList->firstNode);
		Parser* p = new Parser(parserInfo);
		TreeNode* firstTreeNode = p->parse();
		printTree(firstTreeNode);
	}
	//parser


	return 0;
}
