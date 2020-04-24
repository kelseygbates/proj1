#include <iostream>
#include "myshell.h"
using namespace std;

int main() {
	myshell shell;
	shell.readInput();
	shell.parseCommand();
	shell.runSingleCmnd();
	return 0;
}
