/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/
#include"analyzer.h"

void preTransversal(SymbolTable* st, TreeNode* td);
void postTransversal(TreeNode* td);

Analyzer::Analyzer() {
	this->td = NULL;
}

Analyzer::Analyzer(TreeNode* td) {
	this->td = td;
}

//true if error found 
static bool isError = false;

SymbolTable* Analyzer::start() {
	cout << endl << ":)  Analyzer: " << endl << endl;

	//top symbol table
	SymbolTable* top = new SymbolTable();
	//built-in function
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "printnum"));
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "printstr"));
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "inputnum"));
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "inputstr"));
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "str2num"));
	top->insert_dcl(new TreeNode(FUN_DCL, INT_TYPE, "num2str"));

	cout <<endl<< "Errors in pre-transversal: " << endl ;
	preTransversal(top, td);
	cout << endl << "Errors in post-transversal: " << endl;
	postTransversal(td);

	if (isError) {
		return NULL;
	}

	return top;
}

//type checking
void postTransversal(TreeNode* td) {
	TreeNode* temp = td;
	if (temp->nodeKind == PRAMA_LIST_ND || temp->nodeKind == PROGRAM_NO) {
		while (temp->rSibling != NULL)
		{
			temp = temp->rSibling;
			postTransversal(temp);

		}
	}
	else {
		int i = 0;
		//tranversal all children
		while (temp->child[i] != NULL) {
			postTransversal(temp->child[i]);
			i++;
		}

		if (temp->nodeKind == EXPRE_ND) {

			//useless node
			if (temp->child[1] == NULL&&temp->child[0]!=NULL&&temp->epx!=ARRAY_EXPR&&temp->epx!=CALL_EXPR) {
				temp->epx_type = temp->child[0]->epx_type;
				return;
			}

			//array expression and index checking
			if (temp->epx == ARRAY_EXPR) {
				if (temp->child[0] != NULL) {
					if (temp->child[0]->epx_type == INT_TYPE) {
						temp->epx_type = temp->dcl_type;
					}
					else {
						semantic_error_report(temp, "Index of array should be an integer");
					}
				}
				else {
					temp->epx_type = ARRAY_TYPE;
				}
			}

			switch (temp->epx)
			{
			//uncompleted call function
			case CALL_EXPR:
            case CALL_EXPR_ARGS:
			{
				break;
			}
			case CONST_EXPR: {
				break;
			}

			//identifier referencess
			case ID_EXPR:
			case ARRAY_EXPR:
			{
				BucketList* bucket = ((BucketList*)temp->something);
				if (bucket != NULL) {
					temp->epx_type = bucket->td->epx_type;
					if (bucket->td->dcl_type != DEFAULT_TYPE) {
						temp->epx_type = bucket->td->dcl_type;//set as declaration type
					}
				}
				break;
			}
			
			case ASSIGN_EXPR: {
				if (temp->child[0]->epx_type == DEFAULT_TYPE) {//dynamic type
					temp->child[0]->epx_type = temp->child[1]->epx_type;
				}
				else if (temp->child[0]->epx_type != temp->child[1]->epx_type) {//declared type
					semantic_error_report(temp, "Assign wrong type");
				}
				temp->epx_type = temp->child[0]->epx_type;
				break;
			}

			case OP_EXPR: {
				switch (temp->epx_op.getType())
				{
					//boolean equation, and or
					case AND:
					case OR:
					case NOT: {
						if (temp->child[0]->epx_type == BOOL_TYPE&& temp->child[1]->epx_type== BOOL_TYPE) {
							temp->epx_type = BOOL_TYPE;
						}
						else {
							semantic_error_report(temp, "the bool equation should consist with comparing type");
						}
						break;
					}
					//relational equation, > <
					case LT:
					case LTE:
					case GT:
					case GTE:
					case EQUAL:
					case NOT_EQUAL: {
						if (temp->child[0]->epx_type == temp->child[1]->epx_type) {
							temp->epx_type = BOOL_TYPE;
						}
						else {
							semantic_error_report(temp, "two operands of relational expression should be the same type");
						}
						break;
					}
					//calculating equation, + -
					default:
					{
						int i = 0;
						bool isStr = false;
						bool isNum = false;
						bool isInt = false;
						while (temp->child[i] != NULL)
						{
							switch (temp->child[i]->epx_type)
							{
							case INT_TYPE:
								isInt = true;
								break;
							case NUM_TYPE:
								isNum = true;
								break;
							case STR_TYPE:
								isStr = true;
								break;
							default:
								break;
							};
							i++;
						}
						if (isNum && !isStr) {
							temp->epx_type = NUM_TYPE;
						}
						else if (isInt && !isStr) {
							temp->epx_type = INT_TYPE;
						}
						else if (isStr && !isInt && !isNum) {
							temp->epx_type = INT_TYPE;
						}
						else {
							semantic_error_report(temp, "Type is not match");
						}
						break;
					}	
				}
			}
				break;
			default:
				break;
			}
		}
	}
}


//name of function cannot be the same as variable, vice versa
void preTransversal(SymbolTable* st,TreeNode* td) {
	SymbolTable* current = st;
	TreeNode* temp = td;
	if (temp->nodeKind == PRAMA_LIST_ND || temp->nodeKind==PROGRAM_NO) {
		while (temp->rSibling!=NULL)
		{
			temp = temp->rSibling;
			preTransversal(current, temp);
		}
	}
	else {
		int i = 0;

		//compound statement, create a new scope
		if (temp->nodeKind == CPD_ND&&temp->parent->dcl!=FUN_DCL) {
			current = new SymbolTable(temp, current);
		}
		//function declaration
		if (temp->nodeKind == DCL_ND && temp->dcl == FUN_DCL) {
			if (current->insert_dcl(temp)) {
				isError = true;
			}
			current = new SymbolTable(temp, current);
			preTransversal(current, temp->child[0]);
			preTransversal(current, temp->child[1]);
			return;
		}
		//insert declaration
		if (temp->nodeKind == DCL_ND) {
			if (current->insert_dcl(temp)) {
				isError = true;
			}
		}
		//insert epression usage
		if (temp->nodeKind == EXPRE_ND) {
			if (current->insert_ref(temp)) {
				isError = true;
			}
		}

		//tranversal all children
		while (temp->child[i] != NULL) {
			preTransversal(current, temp->child[i]);
			i++;
		}
	}
}
