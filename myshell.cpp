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

// not sure if we need a destructor for these arrays cuz we didnt call 'new' when we made them
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
		pos += strlen(argv[i]) + 1; //point to next token
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
	pid_t pid;
	// step 1: create n - 1 commands if n > 0 where n is number of commands
	int pipeFds[commands - 1];
	// is this the correct way to create n - 1 commands?
	for(int i = 0; i < commands - 1; i++) {
		status = pipe(&pipeFds[i]);
		if(status < 0) {
			perror("could not pipe");
		}
	}
	for(int i = 0; i < commands; i++) {
		pid = fork();
		if(pid == 0) {
			// child process
			if(i - 1 < 0) { // if first process, in: stdin, out: fd[0]
				dup2(pipeFds[i], 1); // redirects output to fd[0]
			} else if(i >= commands - 1) { // if last process, in: fd[i-1], out: stdout
				dup2(pipeFds[i-1], 0); // takes input from fd[i-1]
			} else { // otherwise, take input from previous child and send output to next
				dup2(pipeFds[i-1], 0); // take input from fd[i-1]
				dup2(pipeFds[i], 1);  // send output to fd[i]
			}
			// child closes all pipe file descriptors
			// should I do it this way?
			for(int j = 0; j < commands - 1; j++) {
				close(pipeFds[j]);
			}
			// construct args
			parseCommand(i);
			execvp(argv[0], argv);
			perror("execvp error");
			exit(1);
		} else if(pid > 0) {
			// do nothing
			// why do we do nothing here?
			// dont we have to wait on the sub-child processes?
		} else {
			perror("an error occured");
		}
	}

	// parent closes all commands
	for(int j = 0; j < commands - 1; j++) {
		close(pipeFds[j]);
	}
	// parent waits on all child processes
	for(int i = 0; i < commands - 1; i++) {
		waitpid(pid, &status, 0);
		cout << endl << "Process " << pid << " exits with status " << status << endl;
	}
}
