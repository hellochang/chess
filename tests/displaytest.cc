#include <iostream>
#include "textdisplay.h"
#include "move.h"
using namespace std;

int main() {
	View *A = new TextDisplay(false);
	A->print(cout);
	//Move hello("d7", "d5");
	A->notify(5,5,'K');
	A->print(cout);
	delete A;
}
