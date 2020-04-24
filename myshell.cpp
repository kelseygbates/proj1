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
	char cline[50];
	getline(cin, line);
	strcpy(cline, line.c_str());
	for (int i = 0; i < 10; i++) {
		found = line.find('|');
		if (found != string::npos) { // if pipe character was not the last character in input string
			pipes++;
			for (int j = 0; j < found; j++) {
				if (isspace(cline[j])) {
					//cout << "Setting buf[" << i << "][" << j << "] to \0" << endl;
					argCt[i]++;
					buf[i][j] = '\0';
				} else {
					//cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			line.erase(0, found + 2); // index past | and the space following it
			//cout << "Remaining characters in line: " << line << endl;
		} else { // no pipe character or its the last char in the input string
			cout << "no pipe found!" << endl;
			for (int j = 0; j < line.length(); j++) {
				if (isspace(cline[j])) {
					//cout << "Setting buf[" << i << "][" << j << "] to \0" << endl;
					argCt[i]++;
					buf[i][j] = '\0';
				} else {
					//cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			buf[i][line.length()] = '\0'; // need this to inset final null terminating character
			i = 10; // to end the loop
		}
	}
}

void myshell::parseCommand() {
	int pos = 0;
	for (int i = 0; i < argCt[completed] + 1; i++) {
		argv[i] = (char *) &buf[completed][pos];
		//cout << strlen(argv[i]) << endl;
		pos += strlen(argv[i]) + 1; //point to next token
	}
	argv[argCt[completed] + 1] = (char *) NULL;
}


void myshell::runPipeCmnd() {
	int fds[2]; // file desciptors
	//int status; // to show status of child
	int pipeRs;
	pipeRs = pipe(fds);
	if(pipeRs < 0) {
		perror("Piping error");
	}
	pipeRs = fork();
	//pid_t pid;
	if(pipeRs == 0) {
		// child process
		dup2(fds[0], 0);
		close(fds[1]);
		execvp(argv[0], argv);
		perror("failed");
	} else {
		// parent process
		dup2(fds[1], 1);
		close(fds[0]);
		execvp(argv[0], argv);
		perror("failed");
	}

}

void myshell::runSingleCmnd() {
	pid_t pid;
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
		completed++;
		cout << endl << "Process " << pid << " exits with status " << status << endl;
	}
}

