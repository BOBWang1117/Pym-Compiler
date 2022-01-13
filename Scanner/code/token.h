#include <string>

using namespace std;

//names of token
//IMPORTANT: if the token here is modified, change the array in the token.cpp file
typedef enum  // CANNOT CHANGE THE ORDER!
{	
	KEYWORDS_INT, KEYWORDS_NUM, STR, IF, ELSE, ELIF, WHILE, FOR, DEF, RETURN, AND, OR, NOT,IN,
	ID,
	NEWLINE, // \n
	LPR, // (
	RPR, // )
	LBR, // [
	RBR, // ]
	LCUR,// {
	RCUR,// }
	ADD, // +
	SUB, // -
	MUL, // *
	DIV, // /
	MOD, // %
	LT,	// <
	LTE, // <=
	GT, // >
	GTE, // >=
	ASSIGN, // =
	EQUAL, // ==
	NOT_EQUAL, // !=
	COMMA, // ,
	COLON, //:
	SEMI, //;
	RETURN_TYPE, //->
	SUB_ASSIGN,	//-=
	ADD_ASSIGN,	//+=
	ENDF,
	ERROR,
	INDENT,
	DEDENT,
	DEFAULT,
	INT,
	NUM
} TokenType;

//c++
class Token {
private:
	TokenType type;
	string content;
public:
	TokenType getType();
	void setType(TokenType);
	string getContent();
	void setContent(string content);
	string toString();
	Token();
	Token(TokenType);
	Token(TokenType, string);
};
