//
// Created by Kelsey Bates on 4/23/20.
//
#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <unistd.h>
using namespace std;
#include "myshell.h"

myshell::myshell() {
	// make argCt an array of integers, one for each pipe
	// initialize argCt to 0 for all pipes
	for(int i = 0; i < MAX_ARGS; i++) {
		argCt[i] = 0;
	}
	// track completed processes
	completed = 0;
	// track pipes needed
	pipes = 0;
	cout << "myshell$";
}

void myshell::readInput() {
	size_t found;
	string line;
	int lineLen = line.length();
	char cline[50];
	getline(cin, line);
	strcpy(cline, line.c_str());
	for (int i = 0; i < 10; i++) {
		found = line.find('|');
		cout << found << endl;
		if (found < string::npos) { // if pipe character was not the last character in input string
			pipes++;
			cout << "pipes found: " << pipes << endl;
			for (int j = 0; j < found; j++) {
				if (isspace(cline[j])) {
					cout << "Setting buf[" << i << "][" << j << "] to backslash 0" << endl;
					argCt[i]++;
					buf[i][j] = '\0';
				} else {
					cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			line.erase(0, found + 2); // index past | and the space following it
			strcpy(cline, line.c_str());
			//cout << "Remaining characters in line: " << line << endl;
		} else { // no pipe character or its the last char in the input string
			cout << "no pipe found!" << endl;
			argCt[i] = 1;
			strcpy(cline, line.c_str());
			for (int j = 0; j < line.length(); j++) {
				if (isspace(cline[j])) {
					if(j != lineLen) {
						cout << "Setting buf[" << i << "][" << j << "] to backslash 0" << endl;
						argCt[i]++;
						buf[i][j] = '\0';
					}
				} else {
					cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			cout << "Setting buf[" << i << "][" << lineLen << "] to backslash 0" <<  endl;
			buf[i][lineLen] = '\0'; // need this to inset final null terminating character
			i = 10; // to end the loop
		}
	}
	cout << "argct for arg 0 " << argCt[0] << endl;
	cout << "argct for arg 1 " << argCt[1] << endl;
	cout << "argct for arg 2 " << argCt[2] << endl;


}

char* myshell::parseCommand(int cmdIdx) {
	int pos = 0;
	cout << "making argv for command " << cmdIdx << " " << endl;
	char* argv[MAX_ARGS];
	printf("%s\n", buf[cmdIdx]);
	for (int i = 0; i < argCt[cmdIdx]; i++) {
		argv[i] = (char *) &buf[cmdIdx][pos];
		cout << "making argv[" << i << "] = ";
		printf("%s\n", (char*)&buf[cmdIdx][pos]);
		//cout << strlen(argv[i]) << endl;
		pos += strlen(argv[i]) + 1; //point to next token
	}
	cout << "making argv[" << argCt[cmdIdx] << "] = null" << endl;
	argv[argCt[cmdIdx]] = (char *) NULL;
	return *argv;
}


void myshell::runPipeCmnd() {
//	int fds[2]; // file desciptors
//	//int status; // to show status of child
//	int pipeRs;
//	pipeRs = pipe(fds);
//	if(pipeRs < 0) {
//		perror("Piping error");
//	}
//	pipeRs = fork();
//	//pid_t pid;
//	if(pipeRs == 0) {
//		// child process a
//		dup2(fds[0], 0);
//		close(fds[1]);
//		execvp(argv[0], argv);
//		perror("failed");
//	} else {
//		// child process b
//		dup2(fds[1], 1);
//		close(fds[0]);
//		execvp(argv[0], argv);
//		perror("failed");
//	}

}

void myshell::runSingleCmnd() {
	pid_t pid;
	char* args = parseCommand(0);
	int status;
	pid = fork();
	if (pid < 0) {
		perror("Error pid < 0");
	} else if (pid == 0) {
		execvp((const char*)&args[0], (char *const *)&args);
		perror("execvp error");
		exit(1);
	} else {
		waitpid(pid, &status, 0);
		completed++;
		cout << endl << "Process " << pid << " exits with status " << status << endl;
	}
}

int myshell::makePipes() {
	//pid_t pid;
	cout << "enter makePipes" << endl;
	int in = 0;
	int pipeStatus;
	int fd[2];
	int i;
	for(i = 0; i < pipes - 1; i++) {
		pipeStatus = pipe(fd);
		if(pipeStatus < 0) {
			cout << "pipe error" << endl;
		}
		pipeStatus = makePipedProcess(in, fd[1], i);
		cout << "pipe status " << pipeStatus << endl;
		close(fd[1]);
		in = fd[0];
	}
	if(in != 0) {
		dup2(in, 0);

	}
	char* args = parseCommand(i);
	cout << args << endl;
	//pipeStatus = execvp(&buf[i][0], (char *const *)buf[i]);
	pipeStatus = execvp((const char*)&args[0], (char *const *)&args);

	perror("execvp error in makePipes");
	return pipeStatus;
}

int myshell::makePipedProcess(int in, int out, int cur) {
	pid_t pid;
	int status = -1;
	if((pid = fork()) == 0) {
		if(in != 0) {
			dup2(in, 0);
			close(in);
		}
		if(out != 1) {
			dup2(out, 1);
			close(out);
		}
		char* args = parseCommand(cur);
		//pid = execvp((const char*)&buf[cur][0], (char *const *)&buf[cur]);
		pid = execvp((const char*)&args[0], (char *const *)&args);
		perror("execvp error in makePipedProcess");

	} else {
		waitpid(pid, &status, 0);
		completed++;
		cout << endl << "Process " << pid << " exits with status " << status << endl;
	}
	return pid;

}