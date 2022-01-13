/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include "util.h"

void TreeNode::AssignNode(TreeNode* parent) {
	this->parent = parent;
}

void TreeNode::AssignNode(TreeNode* parent, TreeNode* lSibling) {
	this->parent = parent;
	this->lSibling = lSibling;
}

void TreeNode::AssignNode(TreeNode* parent, TreeNode* lSibling, TreeNode* rSibling) {
	this->parent = parent;
	this->lSibling = lSibling;
	this->rSibling = rSibling;
}

void TreeNode::AssignChild(TreeNode* child) {
	if (child != NULL) {
		this->child[this->childNum] = child;
		this->childNum++;
	}
}

TreeNode::TreeNode() {
	childNum = 0;
}

TreeNode::TreeNode(NodeKind nk) {
	childNum = 0;
	this->nodeKind = nk;
}

//declare
TreeNode::TreeNode(DclKind dk, ExprType et, string name) {
	childNum = 0;
	this->nodeKind = DCL_ND;
	this->dcl = dk;
	this->dcl_type = et;
	this->dcl_name = name;
}
//array dclare
TreeNode::TreeNode(DclKind dk, ExprType et, string name, int size) {
	childNum = 0;
	this->nodeKind = DCL_ND;
	this->dcl = dk;
	this->dcl_type = et;
	this->dcl_size = size;
	this->dcl_name = name;
}

//expression
TreeNode::TreeNode(ExpressionKind ek, Token t, int constant, string name) {
	childNum = 0;
	this->nodeKind = EXPRE_ND;
	this->epx= ek;
	this->epx_op = t;
	this->epx_expressionID = name;
	this->epx_constant = constant;
}

//error
TreeNode::TreeNode(NodeKind nk,string errorMessage) {
	childNum = 0;
	this->nodeKind = ERROR_ND;
	this->errorMessage = errorMessage;
}

bool checkToken(ParserInfo* info, Token tkp) {
	if (info->currentTokenNode->getToken().getType() == tkp.getType()) {
		return true;
	}
	else {
		return false;
	}
}

bool isToken(ParserInfo* info, Token tkp, string errorMessage) {
	if (info->currentTokenNode->getToken().getType() == tkp.getType()) {
		return true;
	}
	else
	{
		info->errorMessage += errorMessage;
		return false;
	}
}

string genErrorMessage(string error, TokenNode* tn) {
	return error + " At Line " + to_string(tn->getRow());
}

TokenNode* move(ParserInfo* info) {
	TokenNode* n = info->currentTokenNode;
	info->currentTokenNode = info->currentTokenNode->getNext();
	return n;
}

bool need_token_move(ParserInfo* info, Token tkp, string errorMsg) {
	if (info->currentTokenNode->getToken() == tkp) {
		move(info);
		return true;
	}
	else {
		info->errorMessage += errorMsg;
		return false;
	}
}


//Parser
ParserInfo::ParserInfo() {
	this->currentTokenNode = NULL;
	this->currentTreeNode = NULL;
	errorMessage = "";
}
ParserInfo::ParserInfo(TokenNode* tokenNode) {
	this->currentTokenNode = tokenNode;
	this->currentTreeNode = NULL;
	errorMessage = "";
}
ParserInfo::ParserInfo(TokenNode* tokenNode,TreeNode* treeNode) {
	this->currentTokenNode = tokenNode;
	this->currentTreeNode = treeNode;
	errorMessage = "";
}


//contructor
Parser::Parser()
{
	this->parserInfo = NULL;
	info = NULL;
}

Parser::Parser(ParserInfo* parserInfo) {
	this->parserInfo = parserInfo;
	this->info = NULL;
}

Parser::Parser(ParserInfo* parserInfo, void* info) {
	this->parserInfo = parserInfo;
	this->info = info;
}

/* returning a parse tree, based on the tokenList that the parser knows */
TreeNode* Parser::parse() {
	bool status=false;
	return program(this->parserInfo,&status);
}

string printSpace(int indent) {
	string s = "";
	for (int i = 0; i < indent-1; i++) {
		s += "  ";
	}
	s += "  |";
	return s;
}

string printDclType(ExprType t) {
	switch (t)
	{
	case INT_TYPE: {
		return "int";
		break;
	}
	case NUM_TYPE: {
		return "num";
		break;
	}
	case STR_TYPE: {
		return "str";
		break;
	}
	
	default:
		return "";
		break;
	}
}

//status will be true if error exists
void printTree(TreeNode* tree, bool* status) {
	cout << endl << "--------------------------------------" << endl;
	cout << ":) The Parse Tree Looks Like: " << endl << endl;
	printTree(tree, 1, status);
	cout <<  endl << ":) Print Finish " << endl ;
	cout << "--------------------------------------" << endl;
}

//print
void printTree(TreeNode* tree,int indent,bool * status) {
	switch (tree->nodeKind)
	{
		case PROGRAM_NO: {
			while (tree->rSibling!=NULL)
			{
				printTree(tree->rSibling,indent, status);
				tree = tree->rSibling;
			}
			break;
		}
		case STMT_ND: {
			int i = 0;
			TreeNode** children = tree->child;
			while (children[i] != NULL) {
				printTree(children[i],indent, status);
				i++;
			}
			break;
		}
		case DCL_ND: {
			switch (tree->dcl)
			{
			case FUN_DCL: {
				cout << printSpace(indent) << "Declare: function " << tree->dcl_name;
				if (tree->child[0]->nodeKind== PRAMA_LIST_ND) {
					cout << " with parameters:" <<"  (At Line "<<tree->lineNum<<")"<<endl;

					TreeNode* param = tree->child[0]->rSibling;
					while (param!=NULL)
					{
						printTree(param,indent+1, status);
						param = param->rSibling;
					}

					//compound
					printTree(tree->child[1], indent + 1, status);
				}
				if (tree->child[0]->nodeKind == CPD_ND) {
					cout <<"  (At Line "<<tree->lineNum<<")"<<endl;
					printTree(tree->child[0], indent + 1, status);
				}
				break;
			}
			case VAR_DCL: {
				cout << printSpace(indent) << "Declare: " << printDclType(tree->dcl_type) << " " << tree->dcl_name <<"  (At Line "<<tree->lineNum<<")"<<endl;
				break;
			}
			case ARRAY_DCL: {
				cout << printSpace(indent) << "Declare: " << printDclType(tree->dcl_type) << " " << tree->dcl_name <<" [" << ((tree->dcl_size==0)?"":to_string(tree->dcl_size)) << "]"<<"  (At Line "<<tree->lineNum<<")"<<endl;
				break;
			}
			case PARAM_VAR_DCL: {
				cout << printSpace(indent) << "Parameter: " << printDclType(tree->dcl_type) << " " << tree->dcl_name  <<"  (At Line "<<tree->lineNum<<")"<<endl;
				break;
			}
			case PARAM_ARRAY_DCL: {
				cout << printSpace(indent) << "Parameter: " << printDclType(tree->dcl_type) << " " << tree->dcl_name << " [" << ((tree->dcl_size == 0) ? "" : to_string(tree->dcl_size)) << "]" <<"  (At Line "<<tree->lineNum<<")"<<endl;
				break;
			}
			default:
				cout << printSpace(indent) << "NULL";
				break;
			}
			break;
		}
		case CPD_ND: {
			cout << printSpace(indent) << "Compound Stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;
			int i = 0;
			while (i < MAX_CHILDREN - 1 && tree->child[i] != NULL) {
				printTree(tree->child[i],indent+1, status);
				i++;
			}
			break;
		}
		case IF_ND: {
			cout << printSpace(indent) << "If Stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;

			//expression
			printTree(tree->child[0], indent + 1, status);

			//compound
			printTree(tree->child[1], indent + 1, status);

			//elif stmt or else stmt
			if (tree->child[2] != NULL) {
				printTree(tree->child[2], indent + 1, status);
			}

			//else stmt
			if (tree->child[3] != NULL) {
				printTree(tree->child[3], indent + 1, status);
			}
			break;
		}
		case ELIF_ND: {
			cout << printSpace(indent) << "Elif Stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;

			int i = 0;
			while (tree->child[i] != NULL) {
				printTree(tree->child[i], indent, status);
				i++;
			}
			break;
		}
		case ELSE_ND: {
			cout << printSpace(indent) << "Else Stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;

			int i = 0;
			while (tree->child[i] != NULL) {
				printTree(tree->child[i], indent, status);
				i++;
			}
			break;
		}
		case WHILE_ND: {
			cout << printSpace(indent) << "While Stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;
			
			int i = 0;
			while (tree->child[i] != NULL) {
				printTree(tree->child[i], indent+1, status);
				i++;
			}
			break;
		}
		case RETURN_ND: {
			cout << printSpace(indent) << "Return" <<"  (At Line "<<tree->lineNum<<")"<<endl;

			//expression
			if(tree->child[0] != NULL)
				printTree(tree->child[0], indent + 1, status);
			break;
		}

		case EXPRE_ND: {

			switch (tree->epx)
			{

			case OP_EXPR: {
				cout << printSpace(indent) << "Operator: ";
				switch (tree->epx_op.getType())
				{
				case OR: {
					cout << "or" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent+1, status);
						i++;
					}
					break;
				}
				case AND: {
					cout << "and" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case NOT: {
					cout << "not" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case LT: {
					cout << "<" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case LTE: {
					cout << "<=" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case GT: {
					cout << ">" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case GTE: {
					cout << ">=" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case EQUAL: {
					cout << "==" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case NOT_EQUAL: {
					cout << "!=" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case ADD: {
					int i = 0;
					if (tree->childNum == 1) {
						cout << "+ (positive)" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					else if (tree->childNum == 2) {
						cout << "+ (addition)" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					else {
						cout << "+" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case SUB: {
					if (tree->childNum == 1) {
						cout << "- (negative)" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					else if (tree->childNum == 2) {
						cout << "- (subtraction)" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					else {
						cout << "-" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					}
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				case MUL: {
					cout << "*" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}				
				case DIV: {
					cout << "/" <<"  (At Line "<<tree->lineNum<<")"<<endl;
					int i = 0;
					while (tree->child[i] != NULL) {
						printTree(tree->child[i], indent + 1, status);
						i++;
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case CONST_EXPR: {
				switch (tree->epx_type)
				{
				case INT_TYPE: {
					cout << printSpace(indent) << "Integer: "<<tree->epx_constant<<"  (At Line "<<tree->lineNum<<")"<<endl;
					break;
				}
				case NUM_TYPE: {
					cout << printSpace(indent) << "Num: " << tree->epx_constant <<"  (At Line "<<tree->lineNum<<")"<<endl;
					break;
				}
				case STR_TYPE: {
					cout << printSpace(indent) << "String: " << tree->epx_constant <<"  (At Line "<<tree->lineNum<<")"<<endl;
					break;
				}
				default:
					break;
				}
				break;
			}
			case ID_EXPR: {
				cout << printSpace(indent) << "ID: " << tree->epx_expressionID <<"  (At Line "<<tree->lineNum<<")"<<endl;
				break;
			}
			case ARRAY_EXPR: {
				cout << printSpace(indent) << "ARRAY ID: " << tree->epx_expressionID << "  (At Line " << tree->lineNum << ")" << endl;
				printTree(tree->child[0], indent + 1, status);
				break;
			}
			case CALL_EXPR: {
				cout << printSpace(indent) << "Call Function: " << tree->epx_expressionID <<"  (At Line "<<tree->lineNum<<")"<<endl;
				printTree(tree->child[0], indent+1, status);
				break;
			}
			case CALL_EXPR_ARGS: {
				int i = 0;
				cout << printSpace(indent) << "Parameter(s): " <<"  (At Line "<<tree->lineNum<<")"<<endl;
				while (tree->child[i] != NULL) {
					printTree(tree->child[i], indent+1, status);
					i++;
				}
				break;
			}
			case ASSIGN_EXPR: {
				cout << printSpace(indent) << "Assignment expression, with LHS and RHS:" <<"  (At Line "<<tree->lineNum<<")"<<endl;
				printTree(tree->child[0], indent + 1, status);
				printTree(tree->child[1], indent + 1, status);
				break;
			}
			default:
				int i = 0;
				if (tree->child[0] != NULL) {
					cout << printSpace(indent) << "Expression stmt:" <<"  (At Line "<<tree->lineNum<<")"<<endl;
				}
				while (tree->child[i] != NULL)
				{
					printTree(tree->child[i], indent + 1, status);
					i++;
				}
				break;
			}
			break;
		}

		case ERROR_ND: {
			cout << printSpace(indent) <<"ERROR FOUND: "<< tree->errorMessage<<endl;
			if (!(*status)) {
				*status = true;
			}
			break;
		}
		default:
			break;
	}
}