#include"tools.h"
#include<vector>

using namespace std;

int main(int argc, char ** argv) {

	//main
	string fileName="";
	if (argc == 2) {
		fileName= argv[1];
	}
	scanner(fileName);

	return 0;
}
