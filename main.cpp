#include <iostream>
#include <fstream>
#include <map>
#include <queue>
using namespace std;

int main()
{
    struct letter {
        char ch;
        int frequency;
        bool operator<(const letter &other) const {
            return frequency < other.frequency; // max-heap based on frequency
        }
    };
    ifstream inputFile("inputText.txt");
    map<char, int> letterFrequency;
    string line;
    priority_queue<letter> pq;

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
        pq.push({pair.first, pair.second});
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
