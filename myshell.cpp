//
// Created by Kelsey Bates on 4/20/20.
//

#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <unistd.h>
using namespace std;


void readInput(char **argv);
bool checkForPipe(int argCt, char** argv, char** command1, char** command2);
void pipe(char** command1, char** command2);
void run(int argCt, char** argv);

const int MAX_ARGS = 10;

int main() {
    char *argv[MAX_ARGS];
//    char *command1[MAX_ARGS];
//    char *command2[MAX_ARGS];
//		char *command3[MAX_ARGS];

		//int argCt;
		//cout << "Hello shell" << endl;
    string line;
    //char buf[10][500];
    //size_t found;
    //while(true) {
		cout << "myshell$";
		readInput(argv);
		//cout << argv[0][0];

//			bool pipeBool = checkForPipe(argCt, argv, command1, command2);
//
//			while(pipeBool) {
//				pipe(command1, command2);
//				pipeBool = checkForPipe(argCt, argv, command2, command3);
//			}
//				run(argCt, argv);

//		getline(cin,line);
//		cout << "input was: " << line << endl;
//		cout << "line length is " << line.length() << endl;
//		for (int i = 0; i < 10; i++) {  // 10 is hardcoded in, might have to change/ make global var.
//            found = line.find("|");
//            cout << "position of first pipe is " << found << endl;
//            if (found != string::npos) {
//                for (int j = 0; j < found; j++) {
//                    buf[i][j] = line[j];
//                }
//                line.erase(0, found + 2); // +2 because need to index past | and next space
//                cout << "remaining characters in line: " << line << endl;
//                cout << "new line length: " << line.length() << endl;
//            } else {
//                for (int j = 0; j < line.length(); j++) {
//                    buf[i][j] = line[j];
//                }
//                i = 10;
//            }
//		}
//        for (int i = 0; i < 10; i++){
//            for (int j = 0; j < 30; j++) {
//                cout << buf[i][j];
//            }
//            cout << endl;
//        }
//
////        char* args[50];
////        int pos = 0;
////        for(int i = 0; i < 3; i++) {
////        	args[i] = (char*)&buf[0][pos];
////        	pos += strlen(args[i]) + 1;
////        }
////        args[3] = (char*)NULL;
////
////        execvp(args[0], args);
////        cout << args[0] << " ";
//				char *av[] = { "ls", "-al", "/", 0 };
//        execvp(av[0], av);
//        perror("failed to run");
//        exit(1);
//    //}
	return 0;
}

// why pointer to char pointer?
void readInput(char **argv) {
	char buf[10][500];
	size_t found;
	string line;
	char cline[50];
	int numArgs;
	//int argCt = 0;
	getline(cin, line);
	strcpy(cline, line.c_str());
	//cout << "Line was " << line << endl;
	for(int i = 0; i < 10; i++) {
	    numArgs = 0;
		found = line.find('|');
		if(found != string::npos) { // if pipe character was not the last character in input string
			for(int j = 0; j < found; j++) {
				if(isspace(cline[j])) {
					//cout << "Setting buf[" << i << "][" << j << "] to \0" << endl;
					numArgs++;
					buf[i][j] = '\0';
				} else {
					//cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			line.erase(0, found + 2); // index past | and the space following it
			cout << "Remaining characters in line: " << line << endl;
		} else { // no pipe character or its the last char in the input string
		    cout << "no pipe found!" << endl;
			for(int j = 0; j < line.length(); j++) {
				if(isspace(cline[j])) {
					//cout << "Setting buf[" << i << "][" << j << "] to \0" << endl;
					numArgs++;
					buf[i][j] = '\0';
				} else {
					//cout << "Setting buf[" << i << "][" << j << "] to " << cline[j] << endl;
					buf[i][j] = cline[j];
				}
			}
			buf[i][line.length()] = '\0'; // need this to inset final null terminating character
			i = 10; // to end the loop
		}

		/*char* argv[50];
		int pos = 0;

        // need to change hard coded 3 so that it works for all number of arguments,
        // have to introduce temp variable to count this?
		for (int i = 0; i < 3; i++) {
			argv[i] = (char*)&buf[0][pos];
			//cout << strlen(argv[i]) << endl;
			pos += strlen(argv[i]) + 1; //point to next token
		}
		argv[3] = (char*)NULL;*/


		pid_t pid;
		int status;
		pid = fork();
        char* argv[50];
        int pos = 0;

        // need to change hard coded 3 so that it works for all number of arguments,
        // have to introduce temp variable to count this?
        for (int i = 0; i < numArgs + 1; i++) {
            argv[i] = (char*)&buf[0][pos];
            //cout << strlen(argv[i]) << endl;
            pos += strlen(argv[i]) + 1; //point to next token
        }
        argv[numArgs + 1] = (char*)NULL;
		if(pid < 0) {
			perror("Error pid < 0");
		} else if (pid == 0) {
			execvp(argv[0], argv);
			perror("Execvp error");
		} else {
			waitpid(pid, &status, 0);
			cout << "Process " << pid << " exited with status " << status << endl;
		}
	}


//	while(cin >> arg) {
//		cString = new char[arg.size()+1];
//		strcpy(cString, arg.c_str());
//		argv[argCt] = cString;
//		argCt++;
//		if(cin.get() == '\n')
//			break;
//	}
//	argv[argCt] = NULL;
//	return argCt;
}

bool checkForPipe(int argCt, char** argv, char** command1, char** command2) {
	bool pipe = false;
	int count = 0;
	int pipeIdx = -1;
	for(int i = 0; i < argCt; i++) {
		if(*argv[i]=='|') {
			pipe = true;
			pipeIdx = i;
		}
	}
	if(pipe) {
		for(int i = 0; i < pipeIdx; i++) {
			command1[i] = argv[i];
		}
		for(int i = pipeIdx+1; i < argCt; i++) {
			command2[count] = argv[i];
			count++;
		}
	}
	command1[pipeIdx] = NULL;
	command2[count] = NULL;
	return pipe;
}

void pipe(char** command1, char** command2) {
	int fds[2];
	int status;
	pipe(fds);
	pid_t pid;
	if(fork() == 0) {
		dup2(fds[0], 0);
		close(fds[1]);
		execvp(command2[0], command2);
		perror("failed");
	} else if((pid = fork()) == 0) {
		dup2(fds[1], 1);
		close(fds[0]);
		execvp(command1[0], command1);
		perror("failed");
	} else {
		status = waitpid(pid, NULL, 0);
		cout << "Process " << pid << " exited with status " << status << endl;
	}
}

void run(int argCt, char** argv) {
	pid_t pid;
	pid = fork();
	if(pid < 0) {
		perror("Error pid < 0");
	} else if (pid == 0) {
		execvp(argv[0], argv);
		perror("Execvp error");
	}
}
