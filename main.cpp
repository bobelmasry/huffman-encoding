#include <iostream>
#include <fstream>
#include <map>
using namespace std;

int main()
{
    ifstream inputFile("inputText.txt");
    map<char, int> letterFrequency;
    string line;

    if (!inputFile) {
        cout << "Error opening Input file" << endl;
        return 1;
    }

    while (getline(inputFile, line)) {
        for (char ch : line) {
            if (isalpha(ch)) {
                ch = tolower(ch);
                letterFrequency[ch]++;
            }
        }
    }
    inputFile.close();

    for (const auto &pair : letterFrequency) {
        cout << pair.first << ": " << pair.second << endl;
    }

    ofstream outputFile("outputFrequency.txt");
    if (outputFile) {
        for (const auto &pair : letterFrequency) {
            outputFile << pair.first << ": " << pair.second << endl;
        }
        outputFile.close();
    } else {
        cout << "Error opening output file" << endl;
    }

    return 0;
}
