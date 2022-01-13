/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"parse.h"

#define ST_SIZE 71
#define SHIFT 2

static int st_id = 0;


class BucketList;
class SymbolTable;

//the identifier reference of a name
class LineList
{
public:
	TreeNode* td;
	LineList* prev;
	LineList* next;
	BucketList* bl;
	LineList();
	LineList(TreeNode* td, BucketList* bl);
	LineList(TreeNode* td, BucketList* bl ,LineList* prev);
	string toString();
};


//stores a declaration of a variable
class BucketList
{
public:
	SymbolTable* st;
	TreeNode* td;
	LineList* lines;
	BucketList* prev;
	BucketList* next;
	void* something;
	BucketList();
	BucketList(SymbolTable* st, TreeNode* td);
	BucketList(SymbolTable* st,TreeNode* td,BucketList* prev);
	string toString(int indent);
};


class SymbolTable
{
public:
	int id;//the unique id of the table/block
	TreeNode* td;//the block corresponding to this symbol table. the TreeNode is compound statement. NULL for the top
	SymbolTable* upper;//to the upper block
	SymbolTable* lower;//to the children blck
	SymbolTable* prev;//sibling block
	SymbolTable* next;//sibling block
	BucketList* hashTable[ST_SIZE];//hash table stores the declaration in the block
	SymbolTable();
	SymbolTable(TreeNode* td);
	SymbolTable(TreeNode* td, SymbolTable* upper);
	bool insert_dcl(TreeNode* dclTd);
	bool insert_ref(TreeNode* refTd);
	BucketList* lookup(string name);
	void toString(int indent);

};

void semantic_error_report(TreeNode* Td, string errorMsg);