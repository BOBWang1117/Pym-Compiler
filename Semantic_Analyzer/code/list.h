/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"token.h"

using namespace std;

//c++
class TokenNode {
private:
    int row;
    Token token;
    TokenNode* prev;
    TokenNode* next;
public:
    int getRow();
    void setRow(int row);
    Token getToken();
    void setToken(Token t);
    TokenNode* getPrev();
    void setPrev(TokenNode*);
    TokenNode* getNext();
    void setNext(TokenNode*);

    TokenNode* findFirst();
    string toString();

    //constructor
    TokenNode();
    TokenNode(int row, Token token);
    TokenNode(int row, Token token, TokenNode* prev);

};

class TokenList {
public:
    TokenList();
    TokenList(TokenNode* firstNode, TokenNode* lastNode, int length);
    TokenList(TokenNode* firstNode, TokenNode* lastNode);
    TokenNode* firstNode;
    TokenNode* lastNode;
    int length;
};