/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu 
  Wang, Hongbo 
*/

#include<iostream>
#include"parse.h"

using namespace std;


TreeNode* stmt(ParserInfo* info, bool* status);
TreeNode* declare_stmt(ParserInfo* info, bool* status);
TreeNode* func_del_stmt(ParserInfo* info, bool* status);
TreeNode* param_list(ParserInfo* info, bool* status);
TreeNode* param(ParserInfo* info, bool* status);
void var_dcl_stmt(ParserInfo* info, bool* status);
void array_dcl_stmt(ParserInfo* info, bool* status);
TreeNode* compound_stmt(ParserInfo* info, bool* status);
TreeNode* if_stmt(ParserInfo* info, bool* status);
TreeNode* elif_part(ParserInfo* info, bool* status);
TreeNode* else_part(ParserInfo* info, bool* status);
TreeNode* while_stmt(ParserInfo* info, bool* status);
TreeNode* return_stmt(ParserInfo* info, bool* status);
TreeNode* expression_stmt(ParserInfo* info, bool* status);
TreeNode* expression(ParserInfo* info, bool* status);

//expression
TreeNode* or_expr(ParserInfo* info, bool* status);
TreeNode* and_not_expr(ParserInfo* info, bool* status);
TreeNode* not_expr(ParserInfo* info, bool* status);
TreeNode* relational_expr(ParserInfo* info, bool* status);
TreeNode* add_min_expr(ParserInfo* info, bool* status);
TreeNode* mul_div_expr(ParserInfo* info, bool* status);
TreeNode* sign_expr(ParserInfo* info, bool* status);
TreeNode* factor(ParserInfo* info, bool* status);
TreeNode* var(ParserInfo* info, bool* status);
TreeNode* call(ParserInfo* info, bool* status);
TreeNode* args(ParserInfo* info, bool* status);
TreeNode* arg_list(ParserInfo* info, bool* status);



//status: return true if the tree node is created successfully, otherwise return false.
TreeNode* program(ParserInfo* info,bool* status) {

	TreeNode* tn = new TreeNode(PROGRAM_NO);
	TreeNode* p_node = tn;
	info->currentTreeNode = tn;
	while (info->currentTokenNode->getToken().getType() != ENDF) {
		info->currentTreeNode=stmt(info, status);
	}
	
	return p_node;
}

TreeNode* stmt(ParserInfo* info, bool* status) {

	TreeNode* t = new TreeNode();
	t->nodeKind = STMT_ND;

	t->lSibling=info->currentTreeNode;
	info->currentTreeNode->rSibling = t;

	info->currentTreeNode = t;

	switch (info->currentTokenNode->getToken().getType())
	{
	case IF: t->AssignChild(if_stmt(info, status)); break;
	case WHILE: t->AssignChild(while_stmt(info, status)); break;
	case RETURN: t->AssignChild(return_stmt(info, status));  break;
	case INDENT: t->AssignChild(compound_stmt(info, status));  break;
	case DEF: t->AssignChild(func_del_stmt(info, status));  break;
	case ID: {
		TokenNode* currentTokenNode = info->currentTokenNode;
		TreeNode* temp = new TreeNode();
		bool newStatus=false;
		temp=declare_stmt(info, &newStatus);
		if (!newStatus) {
			info->currentTokenNode = currentTokenNode;
			temp=expression_stmt(info, &newStatus);
		}
		t->AssignChild(temp);
		break;
	}
	case NEWLINE: {
		move(info);
		info->currentTreeNode = info->currentTreeNode->lSibling;
		return t->lSibling;
	}
	default:
		t->nodeKind=ERROR_ND;
		t->lineNum = info->currentTokenNode->getRow();
		t->errorMessage = genErrorMessage("Unexpected token",info->currentTokenNode);
		*status = false;
		move(info);
		break;
	}

	return t;
}

TreeNode* declare_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, ID, "ID is required in function defination");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage("Identifier is required",info->currentTokenNode));
	}

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent=info->currentTreeNode; //children of statment node
	t->dcl_name=info->currentTokenNode->getPrev()->getToken().getContent();//declare node
	t->nodeKind = DCL_ND;
	info->currentTreeNode = t;//pass to next function

	TokenNode* currentTokenNode = info->currentTokenNode;

	bool newStatus = false;
	array_dcl_stmt(info, &newStatus);
	if (!newStatus) {
		info->currentTokenNode = currentTokenNode;
		var_dcl_stmt(info, &newStatus);
	}
	//NEWLINE
	flag = !(need_token_move(info, NEWLINE, "NEWLINE is missing")||checkToken(info,ENDF)||checkToken(info,DEDENT));
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage("NEWLINE is missing", info->currentTokenNode));
	}

	if (newStatus) {
		*status = true;
		return t;
	}
	else {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "declaration statement construct failure", info->currentTokenNode));
	}
}

void var_dcl_stmt(ParserInfo* info, bool* status) {
	//COLON
	if (checkToken(info, COLON)) {
		move(info);
		TokenNode* tokenNode = move(info);//type
		info->currentTreeNode->lineNum = tokenNode->getRow();
		switch (tokenNode->getToken().getType())
		{
		case INT:
			info->currentTreeNode->dcl = VAR_DCL;
			info->currentTreeNode->dcl_type = INT_TYPE;
			break;
		case NUM:
			info->currentTreeNode->dcl = VAR_DCL;
			info->currentTreeNode->dcl_type = NUM_TYPE;
			break;
		case STR:
			info->currentTreeNode->dcl = VAR_DCL;
			info->currentTreeNode->dcl_type = STR_TYPE;
			break;
		default:
			info->currentTreeNode->errorMessage = genErrorMessage("Variable declaration error", info->currentTokenNode);
			info->currentTreeNode->nodeKind = ERROR_ND;
			*status = false;
			break;
		}
		*status = true;
	}
	else
	{
		*status = false;
	}

}

void array_dcl_stmt(ParserInfo* info, bool* status) {
	if (checkToken(info, COLON)) {
		move(info);
		TokenNode* tokenNode = move(info);//type
		info->currentTreeNode->lineNum = tokenNode->getRow();
		switch (tokenNode->getToken().getType())
		{
		case INT:
			info->currentTreeNode->dcl = ARRAY_DCL;
			info->currentTreeNode->dcl_type = INT_TYPE;
			break;
		case NUM:
			info->currentTreeNode->dcl = ARRAY_DCL;
			info->currentTreeNode->dcl_type = NUM_TYPE;
			break;
		case STR:
			info->currentTreeNode->dcl = ARRAY_DCL;
			info->currentTreeNode->dcl_type = STR_TYPE;
			break;
		default:
			info->currentTreeNode->errorMessage = genErrorMessage("Array declaration error", info->currentTokenNode);
			info->currentTreeNode->nodeKind = ERROR_ND;
			*status = false;
			break;
		}
	}
	else
	{
		*status = false;
	}

	//[]
	TokenType t1 = move(info)->getToken().getType();
	TokenType t2 = move(info)->getToken().getType();
	if (t1 == LBR && t2 == RBR) {
		*status = true;
	}
	else {
		*status = false;
	}
}

TreeNode* func_del_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, DEF, "def is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "def is missing", info->currentTokenNode));
	}

	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode;
	info->currentTreeNode = t;

	flag = !need_token_move(info, ID, "ID is required in function defination");
	t->nodeKind = DCL_ND;
	t->dcl = FUN_DCL;
	t->dcl_name = info->currentTokenNode->getPrev()->getToken().getContent();
	t->lineNum = info->currentTokenNode->getPrev()->getRow();
	flag = !need_token_move(info, LPR, "( is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "Identifier is required or ( is missing", info->currentTokenNode));
	}
	t->AssignChild(param_list(info, status));
	flag = !need_token_move(info, RPR, ") is missing");
	info->currentTreeNode = t;
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( ")  is missing", info->currentTokenNode));
	}

	flag = !need_token_move(info, COLON, ": is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage(":  is missing", info->currentTokenNode));
	}

	flag = !need_token_move(info, NEWLINE, "need a new line");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage("newline is missing", info->currentTokenNode));
	}

	t->AssignChild(compound_stmt(info, status));

	*status = true;
	return t;
}

TreeNode* param_list(ParserInfo* info, bool* status) {
	if (info->currentTokenNode->getToken().getType() == ID) {
		//create new tree node
		TreeNode* t = new TreeNode();
		t->parent = info->currentTreeNode; //children of statment node
		t->nodeKind = PRAMA_LIST_ND;
		t->lineNum = info->currentTokenNode->getRow();
		info->currentTreeNode = t;//pass to next function

		t->rSibling=param(info, status);

		TreeNode* tn = t->rSibling;

		while (info->currentTokenNode->getToken().getType() == COMMA) {
			move(info);
			tn->rSibling=param(info, status);
			tn = tn->rSibling;
		}
		return t;
	}
	else {
		return NULL;
	}
}

TreeNode* param(ParserInfo* info, bool* status) {

	TokenNode* n1 = info->currentTokenNode;
	TokenNode* n2 = n1->getNext();
	TokenNode* n3 = n2->getNext();
	TokenNode* n4 = n3->getNext();
	TokenNode *n5 = n4->getNext();

	//array parameter
	if (n1->getToken().getType() == ID && n2->getToken().getType()==COLON) {
		//create new tree node
		TreeNode* t = new TreeNode();
		t->lSibling = info->currentTreeNode; //children of statment node
		t->nodeKind = DCL_ND;
		t->dcl = PARAM_VAR_DCL;
		t->dcl_name = n1->getToken().getContent();
		t->lineNum = info->currentTokenNode->getRow();
		info->currentTreeNode = t;//pass to next function

		switch (n3->getToken().getType())
		{
		case INT:
			info->currentTreeNode->dcl_type = INT_TYPE;
			break;
		case NUM:
			info->currentTreeNode->dcl_type = NUM_TYPE;
			break;
		case STR:
			info->currentTreeNode->dcl_type = STR_TYPE;
			break;
		default:
			*status = false;
			t= new TreeNode(ERROR_ND, genErrorMessage( "unknown type", info->currentTokenNode));
			return NULL;
			break;
		}
		info->currentTokenNode = n4;
		if (n4->getToken().getType() == LBR) {
			t->dcl = PARAM_ARRAY_DCL;
			if (n5->getToken().getType() != RBR) {
				*status = false;
				t = new TreeNode(ERROR_ND, genErrorMessage( "] is missing", info->currentTokenNode));
				return NULL;
			}
			info->currentTokenNode = n5->getNext();
		}
		return t;
	}
	//parameter
	else if (n1->getToken().getType() == ID ) {
		move(info);
		//create new tree node
		TreeNode* t = new TreeNode();
		t->lSibling = info->currentTreeNode; //children of statment node
		info->currentTreeNode->rSibling = t;//statement node assign children
		t->nodeKind = DCL_ND;
		t->dcl = PARAM_VAR_DCL;
		t->lineNum = info->currentTokenNode->getRow();
		info->currentTreeNode = t;//pass to next function
		t->dcl_name = n1->getToken().getContent();
		return t;
	}
	else {
		return NULL;
	}
}

TreeNode* compound_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, INDENT, "INDENT is required in compound defination");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "INDENT is required in compound defination", info->currentTokenNode));
	}

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	t->nodeKind = CPD_ND;
	t->lineNum = info->currentTokenNode->getRow();
	info->currentTreeNode = t;//pass to next function

	while (info->currentTokenNode->getToken().getType() != DEDENT) {
		TreeNode* tn = stmt(info, status);
		if (tn == info->currentTreeNode) {
			continue;
		}
		info->currentTreeNode = tn;
		t->AssignChild(info->currentTreeNode);
	}

	flag = !need_token_move(info, DEDENT, "DEDENT is required in compound defination");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "DEDENT is required in compound defination", info->currentTokenNode));
	}

	return t;
}

TreeNode* if_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, IF, "IF is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "IF is missing", info->currentTokenNode));
	}

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	t->nodeKind = IF_ND;
	t->lineNum = info->currentTokenNode->getRow();
	info->currentTreeNode = t;//pass to next function

	TreeNode* e = expression(info, status);
	if (e->childNum!=0) {
		t->AssignChild(e);
	}
	else
	{
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "condition expression is missing", info->currentTokenNode));
	}

	flag = !need_token_move(info, COLON, "COLON is missing");
	flag = !need_token_move(info, NEWLINE, "NEWLINE is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "colon and newline is required", info->currentTokenNode));
	}

	t->AssignChild(compound_stmt(info, status));
	t->AssignChild(elif_part(info, status));
	t->AssignChild(else_part(info, status));

	*status = true;
	return t;
}

TreeNode* elif_part(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = checkToken(info, ELIF);
	if (flag) {
		//create new tree node
		TreeNode* t = new TreeNode();
		t->parent = info->currentTreeNode; //children of statment node
		t->nodeKind = ELIF_ND;
		t->lineNum = info->currentTokenNode->getRow();
		info->currentTreeNode = t;//pass to next function

		move(info);
		TreeNode* e = expression(info, status);
		if (e->childNum != 0) {
			t->AssignChild(e);
		}
		else
		{
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "condition expression is missing", info->currentTokenNode));
		}

		flag = !need_token_move(info, COLON, "COLON is missing");
		flag = !need_token_move(info, NEWLINE, "NEWLINE is missing");
		if (flag) {
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "colon and newline is required", info->currentTokenNode));
		}
		t->AssignChild(compound_stmt(info, status));
		t->AssignChild(elif_part(info, status));

		*status = true;
		return t;
	}
	else {
		*status = false;
		return NULL;
	}
}

TreeNode* else_part(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = checkToken(info, ELSE);
	if (flag) {
		//create new tree node
		TreeNode* t = new TreeNode();
		t->parent = info->currentTreeNode; //children of statment node
		t->nodeKind = ELSE_ND;
		t->lineNum = info->currentTokenNode->getRow();
		info->currentTreeNode = t;//pass to next function

		move(info);
		flag = !need_token_move(info, NEWLINE, "NEWLINE is missing");
		if (flag) {
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "newline is required", info->currentTokenNode));
		}
		t->AssignChild(compound_stmt(info, status));
		*status = true;
		return t;
	}
	else {
		*status = false;
		return NULL;
	}
}

TreeNode* while_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, WHILE, "WHILE is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "WHILE is missing", info->currentTokenNode));
	}

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	t->nodeKind = WHILE_ND;
	t->lineNum = info->currentTokenNode->getRow();
	info->currentTreeNode = t;//pass to next function

	TreeNode* e = expression(info, status);
	if (e->childNum != 0) {
		t->AssignChild(e);
	}
	else
	{
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "condition expression is missing", info->currentTokenNode));
	}

	flag = !need_token_move(info, COLON, "COLON is missing");
	flag = !need_token_move(info, NEWLINE, "NEWLINE is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "colon and newline is required", info->currentTokenNode));
	}

	t->AssignChild(compound_stmt(info, status));

	*status = true;
	return t;
}

TreeNode* return_stmt(ParserInfo* info, bool* status) {
	bool flag = false;
	flag = !need_token_move(info, RETURN, "RETURN is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "RETURN is missing", info->currentTokenNode));
	}

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	t->nodeKind = RETURN_ND;
	t->lineNum = info->currentTokenNode->getRow();
	info->currentTreeNode = t;//pass to next function

	if (info->currentTokenNode->getToken().getType() != NEWLINE) {
		t->AssignChild(expression(info, status));
		flag = !need_token_move(info, NEWLINE, "NEWLINE is missing");
		if (flag) {
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "NEWLINE is missing", info->currentTokenNode));
		}
	}

	*status = true;
	return t;
}

//return null if the node fail to construct
TreeNode* expression_stmt(ParserInfo* info, bool* status) {
	TreeNode* t=expression(info, status);
	bool flag = false;
	flag = !need_token_move(info, NEWLINE, "last NEWLINE is missing");
	if (flag) {
		*status = false;
		return new TreeNode(ERROR_ND, genErrorMessage( "last NEWLINE is missing", info->currentTokenNode));
	}

	*status = true;
	return t;
}

//return null if the node fail to construct
TreeNode* expression(ParserInfo* info, bool* status) {

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	t->nodeKind = EXPRE_ND;
	t->lineNum = info->currentTokenNode->getRow();
	info->currentTreeNode = t;//pass to next function

	TokenNode* tn = info->currentTokenNode;

	//expression
	TreeNode* node= or_expr(info, status);

	//declaration
	if (info->currentTokenNode->getToken().getType()==ASSIGN) {

		info->currentTokenNode = tn;//recover token
		info->currentTreeNode = t;//recover parent

		t->AssignChild(var(info, status));
		t->epx = ASSIGN_EXPR;

		bool flag = false;
		flag = !need_token_move(info, ASSIGN, "ASSIGN is missing");
		if (flag) {
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "ASSIGN is missing", info->currentTokenNode));
		}

		info->currentTreeNode = t;//recover parent
		TreeNode* e = expression(info, status);
		if (e != NULL&&e->child[0]!=NULL) {
			t->AssignChild(e);
		}
		else
		{
			*status = false;
			return new TreeNode(ERROR_ND, genErrorMessage( "An expression is missing after the EQUAL note", info->currentTokenNode));
		}

	}
	else {
		t->AssignChild(node);
	}

	*status = true;
	return t;
}


TreeNode* or_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t=and_not_expr(info, status);

	while (info->currentTokenNode->getToken().getType() == OR) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = OR;
		t1->lineNum = info->currentTokenNode->getRow();
		t1->AssignChild(t);
		t->parent = t1;

		t = t1;
		move(info);

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = and_not_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
	}

	return t;
}

TreeNode* and_not_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t = not_expr(info, status);

	while (info->currentTokenNode->getToken().getType() == AND) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = AND;
		t1->lineNum = info->currentTokenNode->getRow();
		t1->AssignChild(t);
		t->parent = t1;

		t = t1;
		move(info);

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = not_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
	}

	return t;

}

TreeNode* not_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t=NULL;

	while (info->currentTokenNode->getToken().getType() == NOT) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = NOT;
		t1->lineNum = info->currentTokenNode->getRow();

		t = t1;
		move(info);

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = relational_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
	}

	if (t == NULL) {
		t = relational_expr(info, status);
	}

	return t;
}

TreeNode* relational_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t = add_min_expr(info, status);

	TokenType token = info->currentTokenNode->getToken().getType();
	while (token == LT || token == LTE || token == GT || token == GTE || token == EQUAL || token == NOT_EQUAL) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = move(info)->getToken();
		t1->lineNum = info->currentTokenNode->getRow();
		t1->AssignChild(t);
		t->parent = t1;

		t = t1;

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = add_min_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
		token = info->currentTokenNode->getToken().getType();
	}

	return t;

}

TreeNode* add_min_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t = mul_div_expr(info, status);

	TokenType token = info->currentTokenNode->getToken().getType();
	while (token == ADD || token == SUB ) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = move(info)->getToken();
		t1->lineNum = info->currentTokenNode->getRow();
		t1->AssignChild(t);
		t->parent = t1;

		t = t1;

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = mul_div_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
		token = info->currentTokenNode->getToken().getType();
	}

	return t;

}

TreeNode* mul_div_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t = sign_expr(info, status);

	TokenType token = info->currentTokenNode->getToken().getType();
	while (token == MUL || token == DIV) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = move(info)->getToken();
		t1->lineNum = info->currentTokenNode->getRow();
		t1->AssignChild(t);
		t->parent = t1;

		t = t1;

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = sign_expr(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
		token = info->currentTokenNode->getToken().getType();
	}

	return t;
}

TreeNode* sign_expr(ParserInfo* info, bool* status) {

	TreeNode* parent = info->currentTreeNode;//save parent tree node
	TreeNode* t = NULL;

	TokenType token = info->currentTokenNode->getToken().getType();
	while (token == ADD || token==SUB ) {
		//create new tree node
		TreeNode* t1 = new TreeNode();
		t1->parent = parent; //children of statment node
		info->currentTreeNode = t1;//pass to next function
		t1->nodeKind = EXPRE_ND;
		t1->epx = OP_EXPR;
		t1->epx_op = move(info)->getToken();
		t1->lineNum = info->currentTokenNode->getRow();
		t->parent = t1;

		t = t1;

		info->currentTreeNode = t;//pass to next function
		TreeNode* t2 = factor(info, status);
		if (t2 != NULL) {
			t->AssignChild(t2);
		}
		token = info->currentTokenNode->getToken().getType();
	}

	if (t == NULL) {
		t = factor(info, status);
	}

	return t;
}

TreeNode* factor(ParserInfo* info, bool* status) {

	TreeNode* t = NULL;

	//(expression)
	if (t == NULL) {
		if (checkToken(info, LPR)) {
			move(info);
			t=expression(info, status);
			if (!checkToken(info, RPR)) {
				t = new TreeNode(ERROR_ND, genErrorMessage( "Right parenthese is missing", info->currentTokenNode));
			}
			else {
				move(info);
			}
		}
	}

	//call
	if (t == NULL) {
		if (checkToken(info, ID)) {
			t = call(info, status);
		}
	}

	//var
	if (t == NULL) {
		if (checkToken(info, ID)) {
			t = var(info, status);
		}
	}

	//CONST
	if (t == NULL) {
		if (checkToken(info, NUM_VAL)) {
			TokenNode* tn= move(info);
			t = new TreeNode();
			t->parent = info->currentTreeNode; //children of statment node
			info->currentTreeNode = t;//pass to next function
			t->nodeKind = EXPRE_ND;
			t->lineNum = info->currentTokenNode->getRow();
			t->epx = CONST_EXPR;
			t->epx_type = NUM_TYPE;
			t->epx_constant = tn->getToken().getContent();
		}
		if (checkToken(info, INT_VAL)) {
			TokenNode* tn = move(info);
			t = new TreeNode();
			t->parent = info->currentTreeNode; //children of statment node
			info->currentTreeNode = t;//pass to next function
			t->nodeKind = EXPRE_ND;
			t->lineNum = info->currentTokenNode->getRow();
			t->epx = CONST_EXPR;
			t->epx_type = INT_TYPE;
			t->epx_constant = tn->getToken().getContent();
		}
		if (checkToken(info, STR)) {
			TokenNode* tn = move(info);
			t = new TreeNode();
			t->parent = info->currentTreeNode; //children of statment node
			info->currentTreeNode = t;//pass to next function
			t->nodeKind = EXPRE_ND;
			t->lineNum = info->currentTokenNode->getRow();
			t->epx = CONST_EXPR;
			t->epx_type = STR_TYPE;
			t->epx_constant = tn->getToken().getContent();
		}
	}

	return t;
}

TreeNode* var(ParserInfo* info, bool* status) {
	TreeNode* t = NULL;
	
	TokenNode* tn = info->currentTokenNode;
	if (checkToken(info, ID)) {
		move(info);
		t = new TreeNode(EXPRE_ND);
		t->parent = info->currentTreeNode; //children of statment node
		info->currentTreeNode = t;//pass to next function
		t->epx = ID_EXPR;
		t->epx_expressionID = tn->getToken().getContent();
		t->lineNum = info->currentTokenNode->getRow();
	}
	if (checkToken(info, LBR)) {
		move(info);
		if(t!=NULL)
			t->epx = ARRAY_EXPR;
		t->AssignChild(expression(info, status));
		if (!need_token_move(info,RBR,genErrorMessage("right bracket is missing",info->currentTokenNode))) {
			t = new TreeNode(ERROR_ND, genErrorMessage("right bracket is missing", info->currentTokenNode));
		}
	}

	return t;
}

TreeNode* call(ParserInfo* info, bool* status) {
	TreeNode* t = NULL;

	TokenNode* tn = info->currentTokenNode;
	if (tn->getToken().getType()==ID && tn->getNext()->getToken().getType()==LPR) {
		t = new TreeNode(EXPRE_ND);
		t->parent = info->currentTreeNode; //children of statment node
		info->currentTreeNode = t;//pass to next function
		t->lineNum = info->currentTokenNode->getRow();
		t->epx = CALL_EXPR;
		t->epx_expressionID = tn->getToken().getContent();
		move(info); move(info);
		t->AssignChild(args(info, status));
		if (!need_token_move(info, RPR, genErrorMessage("right parenthese is missing", info->currentTokenNode))) {
			t = new TreeNode(ERROR_ND, genErrorMessage("right parenthese is missing", info->currentTokenNode));
		}
	}

	return t;
}

TreeNode* args(ParserInfo* info, bool* status) {

	TreeNode* t = arg_list(info, status);

	return t;
}

TreeNode* arg_list(ParserInfo* info, bool* status) {

	//create new tree node
	TreeNode* t = new TreeNode();
	t->parent = info->currentTreeNode; //children of statment node
	info->currentTreeNode = t;//pass to next function
	t->nodeKind = EXPRE_ND;
	t->epx = CALL_EXPR_ARGS;
	t->lineNum = info->currentTokenNode->getRow();
	t->AssignChild(expression(info, status));

	TokenType token = info->currentTokenNode->getToken().getType();
	while (token == COMMA) {
		move(info);//comma

		TreeNode* t1 = expression(info, status);
		if (t1 != NULL) {
			t->AssignChild(t1);
		}
		token = info->currentTokenNode->getToken().getType();
	}

	return t;
}
