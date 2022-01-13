/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"list.h"
using namespace std;

int TokenNode::getRow() {
	return this->row;
}

void TokenNode::setRow(int row) {
	this->row = row;
}

Token TokenNode::getToken(){
	return this->token;
}

void TokenNode::setToken(Token t){
	this->token = t;
}

TokenNode* TokenNode::getPrev(){
	return this->prev;
}

void TokenNode::setPrev(TokenNode* prev){
	this->prev = prev;
}

TokenNode* TokenNode::getNext(){
	return this->next;
}

void TokenNode::setNext(TokenNode* next){
	this->next = next;
}

//return the first token of the list
TokenNode* TokenNode::findFirst() {
	TokenNode* i = this;
	while (i->getPrev() != NULL) {
		i = i->getPrev();
	}
	return i;
}

//constructor
TokenNode::TokenNode(){
	this->setToken(DEFAULT);
	this->setPrev(NULL);
	this->setNext(NULL);
}
TokenNode::TokenNode(int row,Token token){
	this->row = row;
	this->token = token;
	this->setPrev(NULL);
	this->setNext(NULL);
}
TokenNode::TokenNode(int row,Token token,TokenNode* prev){
	this->row = row;
	this->token = token;
	this->setPrev(prev);
	this->setNext(NULL);
}

string TokenNode::toString() {
	return to_string(this->getRow()) + " " + this->getToken().toString();
}


TokenList::TokenList() {
	firstNode = NULL;
	lastNode = NULL;
	length = 0;
}

TokenList::TokenList(TokenNode* firstNode, TokenNode* lastNode, int length) {
	this->firstNode = firstNode;
	this->lastNode = lastNode;
	this->length = length;
}

TokenList::TokenList(TokenNode* firstNode, TokenNode* lastNode) {
	this->firstNode = firstNode;
	this->lastNode = lastNode;
	this->length = 0;
}