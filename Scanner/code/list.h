#include"token.h"

using namespace std;

//c++
class Node {
private:
    int row;
    Token token;
    Node* prev;
    Node* next;
public:
    int getRow();
    void setRow(int row);
    Token getToken();
    void setToken(Token t);
    Node* getPrev();
    void setPrev(Node*);
    Node* getNext();
    void setNext(Node*);

    Node* findFirst();
    string toString();

    //constructor
    Node();
    Node(int row,Token token);
    Node(int row,Token token, Node* prev);

};
