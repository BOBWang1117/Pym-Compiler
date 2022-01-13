/* homework 3. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#ifndef _PARSE_H_
#define _PARSE_H_
#include "tools.h"

// No need for ARG_ND, since an argument is just an expression

/* Naming convention. If the word is no more than 5 character use the full word, like param, array, otherwise use, 3 characters, like varaible -> var, assign -> asn.
 */

typedef enum {DEFAULT_ND, STMT_ND,DCL_ND,PRAMA_LIST_ND, CPD_ND, IF_ND,ELIF_ND,ELSE_ND, WHILE_ND, RETURN_ND, EXPRE_ND, ERROR_ND,PROGRAM_NO} NodeKind;
typedef enum {DEFAULT_DCL,FUN_DCL,PARAM_VAR_DCL,PARAM_ARRAY_DCL, VAR_DCL, ARRAY_DCL} DclKind;
typedef enum {DEFAULT_EXPR,OP_EXPR, CONST_EXPR, ID_EXPR, CALL_EXPR, CALL_EXPR_ARGS, ASSIGN_EXPR} ExpressionKind;
typedef enum {DEFAULT_TYPE,INT_TYPE, NUM_TYPE,STR_TYPE} ExprType;

#define MAX_CHILDREN 2000

/***********   Syntax tree for parsing ************/
class TreeNode {
public:

    void AssignNode(TreeNode* parent);
    void AssignNode(TreeNode* parent, TreeNode* lSibling);
    void AssignNode(TreeNode* parent, TreeNode* lSibling, TreeNode*rSibling);

    void AssignChild(TreeNode* child);

    TreeNode();
    TreeNode(NodeKind);//PROGRAM
    TreeNode(NodeKind,Token);//token node
    TreeNode(DclKind,ExprType,string ,int);//declare
    TreeNode(ExpressionKind, Token, int, string);//expression
    TreeNode(NodeKind, string);//error

    TreeNode * child[MAX_CHILDREN];
    int childNum;
    
    // sibling is useful for declaration_list, param_list, local_declarations, statement_list, arg_list.
    TreeNode* parent;
    TreeNode* lSibling;
    TreeNode* rSibling;

    /* parent is useful to check the containing structure of a node during parsing. So, the connected tree nodes can be found in all directions, up (to parents), down (to children), and horizontally (left and right to siblings).  */
    /* LineNum:  At the momemt in parsing, when this treeNode is constructed, what is the line number of the token being handled. */
    int lineNum; 

    NodeKind nodeKind;

    DclKind dcl; 
    ExprType dcl_type; // used by all dcl and param
    string dcl_name;  // used by all dcl and param
    int dcl_size;     // used by array declaration

    ExpressionKind epx;
    Token epx_op; // used by Op_EXPR
    string epx_constant;      // used by Const_EXPR, saving value
    string epx_expressionID;  // used by ID_EXPR, Call_EXPR, Array_EXPR
    ExprType epx_type;//used by CONST_EXPR, saving type

    /* type is for type-checking of exps, will be updated by type-checker,  the parser does not touch it.  */
    string errorMessage; //can carry something possibly useful for other tasks of compiling
};


class ParserInfo {
public:
    ParserInfo();
    ParserInfo(TokenNode*);
    ParserInfo(TokenNode*,TreeNode*);
    TokenNode* currentTokenNode;
    //TokenNode* currentTokenNode;//unused
    TreeNode* currentTreeNode;
    string errorMessage;
};

/* some tool functions for checking and moving the current token node in the token node list */

/* return True if the tokentype of the token node tkn is tkp, otherwise return FALSE, no error message is reported */
bool checkToken(ParserInfo* info, Token tkp);

/* return True if the tokentype of the token node tkn is tkp,
otherwise return FALSE, and print some message composed using errorMsg */
bool isToken(ParserInfo* info, Token tkp,string errorMessage);

string genErrorMessage(string error,TokenNode* tn);

/* move the position of currentTokenNode in "info" to the next token node in "theTokenList"  in Info, and return the address updated position of the current token node, which is the currentTokenNode info */
TokenNode* move(ParserInfo* info);

/* Combine the need_token and move actions together. If the current token node, currentTokenNode, has the type tkp, return TRUE and move currentTokenNode to its next neighbor, and return TRUE. Otherwise, print some error message using errorMsg and the position of the current token is not moved.
 */
bool need_token_move(ParserInfo*, Token tkp, string errorMsg);


class Parser
{
public:
    Parser();
    Parser(ParserInfo* parserInfo);
    Parser(ParserInfo* parserInfo,void* info);
    TreeNode* parse(); /* returning a parse tree, based on the tokenList that the parser knows */
    void * info; /* Some data belonging to this parser object. It can contain the tokenList that the parser knows.
                For example, the address of  ParserInfo structure can be saved here. */
private:
    ParserInfo* parserInfo;
};

//parse_print

/* TreeNode  is defined in parse.h.
 * If parse.h is not here, but some file includes parse_print.h before including
 * parse.h, then some strange error message appears.  */

void printTree(TreeNode*);
void printTree(TreeNode*,int indent);
string printSpace(int indent);
string printDclType(ExprType);

#endif
