//
// Created by Kelsey Bates on 4/23/20.
//
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <unistd.h>
#include "myshell.h"
using namespace std;

myshell::myshell() {
	// make argCt an array of integers, one for each piped command
	// initialize argCt to 0 for all commands
	for(int i = 0; i < MAX_ARGS; i++) {
		argCt[i] = 0;
	}
	// track commands needed
	commands = 1;
	cout << "myshell$";
}

// not sure if we need a destructor to deallocate these arrays cuz we didnt call 'new' when we made them
//myshell::~myshell() {
//	delete [] argv;
//	delete [] buf;
//	delete [] argCt;
//}

void myshell::readInput() {
	string line;
	char cline[MAX_ARGS];
	getline(cin, line);
	strcpy(cline, line.c_str());
	bool token_start = false;
	int cmds = 0;
	int k = 0;
	for(int j = 0; j < strlen(cline); j++) {
		if (cline[j] == '|') {
			cmds++; // increase row
			commands++; // increase global commands variable
			k = 0;
			token_start = false;
		} else if (cline[j] == ' ') {
			if(token_start) {
				buf[cmds][k++] = '\0';
				token_start = false;
			}
		} else {
			if(!token_start) {
				argCt[cmds]++;
			}
			token_start = true;
			buf[cmds][k++] = cline[j];
		}
	}
	if(token_start) {
		buf[cmds][k] = '\0';
	}
}

void myshell::parseCommand(int cmdIdx) {
	int pos = 0;
	for (int i = 0; i < argCt[cmdIdx]; i++) {
		argv[i] = &buf[cmdIdx][pos];
		pos += strlen(argv[i]) + 1;
	}
	argv[argCt[cmdIdx]] = (char *) NULL;
}

void myshell::runSingleCmnd() {
	pid_t pid;
	int index = 0; // only one command
	parseCommand(index);
	int status;
	pid = fork();
	if (pid < 0) {
		perror("Error pid < 0");
	} else if (pid == 0) {
		execvp(argv[0], argv);
		perror("execvp error");
		exit(1);
	} else {
		waitpid(pid, &status, 0);
		cout << endl << "Process " << pid << " exits with status " << status << endl;
	}
}

void myshell::runPipeCmnd() {
	int status;
	// separate pids for each child process
	pid_t pid[commands];
	// create n - 1 pipes if n > 0 where n is number of commands
	int pipeFds[commands - 1][2];
	for(int i = 0; i < commands - 1; i++) {
		status = pipe(pipeFds[i]);
		if(status < 0) {
			perror("could not pipe");
		}
	}
	for(int i = 0; i < commands; i++) {
		pid[i] = fork();
		if(pid[i] == 0) {
			// child process
			if(i - 1 >= 0) { // if not first process, take input from previous pipe
				dup2(pipeFds[i-1][0], 0);
			}
			if(i < commands - 1) { // if not last process, send output to this pipe
				dup2(pipeFds[i][1], 1);
			}

			// child closes all pipe file descriptors
			for(int j = 0; j < commands - 1; j++) {
				close(pipeFds[j][0]);
				close(pipeFds[j][1]);
			}
			// construct args
			parseCommand(i);
			execvp(argv[0], argv);
			perror("execvp error");
			exit(1);
		} else if(pid[i] > 0) {
			// do nothing
		} else {
			perror("an error occurred");
		}
	}
	// parent closes all file descriptors
	for(int j = 0; j < commands - 1; j++) {
		close(pipeFds[j][0]);
		close(pipeFds[j][1]);
	}
	// parent waits on all child processes
	for(int i = 0; i < commands; i++) {
		waitpid(pid[i], &status, 0);
		cout << endl << "Process " << pid[i] << " exits with status " << status << endl;
	}
}