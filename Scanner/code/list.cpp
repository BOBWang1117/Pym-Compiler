#include"list.h"
using namespace std;

int Node::getRow() {
	return this->row;
}

void Node::setRow(int row) {
	this->row = row;
}

Token Node::getToken(){
	return this->token;
}

void Node::setToken(Token t){
	this->token = t;
}

Node* Node::getPrev(){
	return this->prev;
}

void Node::setPrev(Node* prev){
	this->prev = prev;
}

Node* Node::getNext(){
	return this->next;
}

void Node::setNext(Node* next){
	this->next = next;
}

//return the first token of the list
Node* Node::findFirst() {
	Node* i = this;
	while (i->getPrev() != NULL) {
		i = i->getPrev();
	}
	return i;
}

//constructor
Node::Node(){
	this->setToken(DEFAULT);
	this->setPrev(NULL);
	this->setNext(NULL);
}
Node::Node(int row,Token token){
	this->row = row;
	this->token = token;
	this->setPrev(NULL);
	this->setNext(NULL);
}
Node::Node(int row,Token token,Node* prev){
	this->row = row;
	this->token = token;
	this->setPrev(prev);
	this->setNext(NULL);
}

string Node::toString() {
	return to_string(this->getRow()) + " " + this->getToken().toString();
}
