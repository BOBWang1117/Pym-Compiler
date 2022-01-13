/* homework 3. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"token.h"
using namespace std;


//IMPORTANT: its order should be corresponding to TokenType
string tokenType[] = { 
	"INT", "NUM", "STR", "IF", "ELSE", "ELIF", "WHILE", "FOR", "DEF", "RETURN", "AND", "OR", "NOT","IN",
	"ID",
	"NEWLINE", // \n
	"LPR", // (
	"RPR", // )
	"LBR", // [
	"RBR", // ]
	"LCUR",// {
	"RCUR",// }
	"ADD", // +
	"SUB", // -
	"MUL", // *
	"DIV", // /
	"MOD", // %
	"LT",	// <
	"LTE", // <=
	"GT", // >
	"GTE", // >=
	"ASSIGN", // =
	"EQUAL", // ==
	"NOT_EQUAL", // !=
	"COMMA", // ,
	"COLON", //:
	"SEMI",	//;
	"RETURN_TYPE", //->
	"SUB_ASSIGN",	//-=
	"ADD_ASSIGN",	//+=
	"EOF",
	"ERROR",
	"INDENT",
	"DEDENT",
	"DEFAULT",
	"INT_VAL",
	"NUM_VAL"
};


TokenType Token::getType(){
	return this->type;
}
void Token::setType(TokenType t){
	this->type = t;
}
string Token::getContent(){
	return this->content;
}
void Token::setContent(string content){
	this->content = content;
}
string Token::toString(){
	return tokenType[(int)this->getType()] + " " + this->getContent();
}

Token::Token() {
	this->type = DEFAULT;
	this->content = "";
}

Token::Token(TokenType type) {
	this->type = type;
	this->content = "";
}

Token::Token(TokenType type, string content) {
	this->type = type;
	this->content = content;
}

bool Token::operator==(Token& a1)
{
	if (this->getType() == a1.getType()) {
		return true;
	}
	else {
		return false;
	}
}
