/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"symbolTable.h"

string getIndent(int i) {
	string result = "";
	for (int j = 0; j < i; j++) {
		result += "    ";
	}
	return result;
}

void semantic_error_report(TreeNode* Td,string errorMsg) {
	cout <<"    Error: "<< errorMsg << " at line: "<< Td->lineNum << endl;
}

//line list
LineList::LineList()
{
}

LineList::LineList(TreeNode* td, BucketList* bl) {
	this->td = td;
	this->bl = bl;
}

LineList::LineList(TreeNode* td, BucketList* bl, LineList* prev) {
	this->td = td;
	this->bl = bl;
	this->prev = prev;
}

string LineList::toString() {
	return "|Line List: use "+this->td->epx_expressionID+ "  (Line "+to_string(this->td->lineNum)+")\n";

}



//bucket list
BucketList::BucketList()
{
}

BucketList::BucketList(SymbolTable* st, TreeNode* td) {
	this->st = st;
	this->td = td;
}

BucketList::BucketList(SymbolTable* st, TreeNode* td, BucketList* prev) {
	this->st = st;
	this->td = td;
	this->prev = prev;
}

string BucketList::toString(int indent) {
	string result = "";
	LineList* temp = this->lines;
	while (temp != NULL) {
		result += (getIndent(indent+1)+temp->toString());
		temp = temp->next;
	}
	return getIndent(indent)+ "|Bucket List: declare " + this->td->dcl_name + "  (Line " + to_string(this->td->lineNum) + ")\n"+result;
}


//symbol table
int calHash(string s) {
	int temp = 0;
	int i = 0;
	while (s[i]!='\0')
	{
		temp = ((temp << SHIFT) + s[i]) % ST_SIZE;
		i++;
	}
	return temp;
}

SymbolTable::SymbolTable() {
	this->id = st_id++;

}

SymbolTable::SymbolTable(TreeNode* td) {
	this->id = st_id++;
	this->td = td;

}

SymbolTable::SymbolTable(TreeNode* td, SymbolTable* upper) {
	if (upper->lower != NULL) {
		SymbolTable* lowest = upper->lower;
		while (lowest->next != NULL) {//find the previous sibiling
			lowest = lowest->next;
		}
		this->prev = lowest;
		lowest->next = this;
	}
	else {//first lower
		upper->lower = this;
	}

	this->id = st_id++;
	this->td = td;
	this->upper = upper;
}


bool SymbolTable::insert_dcl(TreeNode* dclTd) {
	if (dclTd == NULL) {
		return false;
	}

	int hash = -1;

	//explicit declaration and function decalaration
	if (dclTd->nodeKind == DCL_ND) {
		if (lookup(dclTd->dcl_name) != NULL) {
			semantic_error_report(dclTd, "Redundent declaration");
			return true;
		}
		hash = calHash(dclTd->dcl_name);
	}

	//assignment without previous declaration
	else if (dclTd->nodeKind == EXPRE_ND && dclTd->parent->nodeKind==EXPRE_ND && dclTd->parent->epx==ASSIGN_EXPR && dclTd->parent->child[0]==dclTd && dclTd->epx_expressionID!="") {
		hash = calHash(dclTd->epx_expressionID);
	}
	
	//not those two previous condition, stop
	if (hash == -1) {
		return false;
	}
	
	//insert new bucket list
	BucketList* bl = new BucketList(this, dclTd);
	if (this->hashTable[hash] == NULL) {
		this->hashTable[hash] = bl;
	}
	else
	{
		BucketList* temp = this->hashTable[hash];
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = bl;
		bl->prev = temp;
	}
	return false;
}

bool SymbolTable::insert_ref(TreeNode* refTd) {
	if (refTd == NULL || refTd->nodeKind != EXPRE_ND|| refTd->epx_expressionID == "") {
		return false;
	}

	SymbolTable* current = this;
	BucketList* bl = NULL;
	while (current != NULL)
	{
		bl = current->lookup(refTd->epx_expressionID);
		current = current->upper;

		//find bl in the nearest scope
		if (bl != NULL) {
			break;
		}
	}

	//not found declaration
	if (bl == NULL) {

		//the left hand side of an assignment statement, implicitly declare
		if (refTd->parent->nodeKind == EXPRE_ND && refTd->parent->epx == ASSIGN_EXPR && refTd->parent->child[0] == refTd) {
			insert_dcl(refTd);
			bl = lookup(refTd->epx_expressionID);
		}
		else {
			semantic_error_report(refTd,"Use a variable before it is declared");
			return true;
		}
	}
	//found declaration
	if(bl!=NULL){
		LineList* line = new LineList(refTd, bl);
		
		if(bl->lines != NULL) {
			LineList* temp = bl->lines;

			//find last line
			while (temp->next!=NULL)
			{
				temp = temp->next;
			}

			line->prev = temp;
			temp->next = line;
		}
		else { //the first time to use
			bl->lines = line;
		}
		refTd->something = bl;
	}
	return false;
}

BucketList* SymbolTable::lookup(string name) {

	int index = calHash(name);

	BucketList* bl = this->hashTable[index];
	while (bl!=NULL)
	{
		if (bl->td->dcl_name == name || bl->td->epx_expressionID == name) {
			return bl;
		}
		bl = bl->next;
	}
	return NULL;
}

void SymbolTable::toString(int indent) {
	string result = "";
	if (this != NULL) {
		cout<< getIndent(indent) + "|Symbol Table " + to_string(this->id) + "\n";
		for (int i = 0; i < ST_SIZE; i++) {
			if (this->hashTable[i] != NULL) {
				cout<< this->hashTable[i]->toString(indent + 1);
			}
		}
	}
	else {
		return;
	}
	this->lower->toString(indent + 1);
	this->next->toString(indent);

}