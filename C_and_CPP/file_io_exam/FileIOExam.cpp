#include <fstream>
#include <iostream>

using namespace std;

// C++ File I/O Example
int main(void) {
    string strFilePath = "exam.txt";

    // write File
    ofstream writeFile(strFilePath.data());
    if(writeFile.is_open()){
        writeFile << "Hello C++!\n";
        writeFile << "File I/O Example\n";
        writeFile.close();
    } else {
        cout << "Can not create file!" << endl;
    }

    // read File
    ifstream readFile(strFilePath.data());
    if(readFile.is_open()){
        string strLine;
        while(getline(readFile, strLine)) {
            cout << strLine << endl;
        }
        readFile.close();
    } else { // if 
        cout << "Not Found File" << endl;
    }

    return 0;
}
