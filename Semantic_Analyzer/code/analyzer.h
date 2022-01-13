/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/
#include"symbolTable.h"

class Analyzer {
public:
	TreeNode* td;
	Analyzer();
	Analyzer(TreeNode*);
	SymbolTable* start();
};