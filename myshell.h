//
// Created by Kelsey Bates on 4/20/20.
//

#ifndef PROJ1_MYSHELL_H
#define PROJ1_MYSHELL_H
const int MAX_ARGS = 10;


class myshell {
public:
		explicit myshell();

		void readInput();

		char* parseCommand(int index);

		void runSingleCmnd();
		void runPipeCmnd();
		int makePipes();
		int makePipedProcess(int in, int out, int cur);
		int completed; // processes completed
		int pipes;

private:
		//char *argv[MAX_ARGS];
		char buf[10][500];
		int argCt[MAX_ARGS];


};
#endif //PROJ1_MYSHELL_H
