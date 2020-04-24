#include <iostream>
#include "myshell.h"
using namespace std;

int main() {
	myshell shell;
	shell.readInput2();

	if(shell.pipes > 0) {
		//shell.makePipes();
	} else {
		shell.runSingleCmnd();
	}

	return 0;
}
