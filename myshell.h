//
// Created by Kelsey Bates on 4/20/20.
//

#ifndef PROJ1_MYSHELL_H
#define PROJ1_MYSHELL_H
const int MAX_ARGS = 1000;
const int MAX_CMDS = 50;

class myshell {
public:
		// Constructs shell object and displays prompt
		explicit myshell();

		//~myshell();

		// Reads the line of user input and stores in a 2d buffer char array
		void readInput();

		// Parses a row of the 2d buffer and formats it for evecvp function
		void parseCommand(int index);

		// Runs a single command, called when there are no pipes needed
		void runSingleCmnd();

		// Runs a pipeline of commands
		void runPipeCmnd();

		// Tracks numbers of commands entered by the user
		int commands;
		
private:
		char buf[MAX_CMDS][MAX_ARGS];
		int argCt[MAX_ARGS];
		char *argv[MAX_ARGS];
};
#endif //PROJ1_MYSHELL_H
