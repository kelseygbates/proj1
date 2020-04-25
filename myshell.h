//
// Created by Kelsey Bates on 4/20/20.
//

#ifndef PROJ1_MYSHELL_H
#define PROJ1_MYSHELL_H
const int MAX_ARGS = 1000;
const int MAX_CMDS = 50;

class myshell {
public:
		explicit myshell();
		//~myshell();
		void readInput();
		void parseCommand(int index);
		void runSingleCmnd();
		void runPipeCmnd();
		int commands;

private:
		char buf[MAX_CMDS][MAX_ARGS];
		int argCt[MAX_ARGS];
		char *argv[MAX_ARGS];
};
#endif //PROJ1_MYSHELL_H
