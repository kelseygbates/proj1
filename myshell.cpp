//
// Created by Kelsey Bates on 4/20/20.
//

#include <iostream>
#include <string>
#include <array>

int main() {
    using namespace std;
		//cout << "Hello shell" << endl;
    string line;
    char buf[10][500];
    size_t found;
    while(true) {
			cout << "myshell$";
			getline(cin,line);
			if(line == "quit") { break; }
			cout << "input was: " << line << endl;
			cout << "line length is " << line.length() << endl;
			for (int i = 0; i < 10; i++) {  // 10 is hardcoded in, might have to change/ make global var.
            found = line.find("|");
            cout << "position of first pipe is " << found << endl;
            if (found != string::npos) {
                for (int j = 0; j < found; j++) {
                    buf[i][j] = line[j];
                }
                line.erase(0, found + 2); // +2 because need to index past | and next space
                cout << "remaining characters in line: " << line << endl;
                cout << "new line length: " << line.length() << endl;
            } else {
                for (int j = 0; j < line.length(); j++) {
                    buf[i][j] = line[j];
                }
                i = 10;
            }
			}
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 30; j++) {
                cout << buf[i][j];
            }
            cout << endl;
        }
    }
	return 0;
}

