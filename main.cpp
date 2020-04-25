#include <iostream>
#include "myshell.h"
using namespace std;

int main() {
	myshell shell;
	shell.readInput();
	if(shell.commands > 1) {
		shell.runPipeCmnd();
	} else {
		shell.runSingleCmnd();
	}
	return 0;
}
