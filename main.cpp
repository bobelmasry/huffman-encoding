#include <fstream>
#include "huffmantree.h"
using namespace std;

#include <filesystem>
#include <iostream>

using namespace std;
using namespace filesystem;

int main()
{
    cout << R"(
               __  __                           ___ _ _        ______
  /\  /\_   _ / _|/ _|_ __ ___   __ _ _ __     / __(_) | ___  / _  (_)_ __  _ __   ___ _ __
 / /_/ / | | | |_| |_| '_ ` _ \ / _` | '_ \   / _\ | | |/ _ \ \// /| | '_ \| '_ \ / _ \ '__|
/ __  /| |_| |  _|  _| | | | | | (_| | | | | / /   | | |  __/  / //\ | |_) | |_) |  __/ |
\/ /_/  \__,_|_| |_| |_| |_| |_|\__,_|_| |_| \/    |_|_|\___| /____/_| .__/| .__/ \___|_|
                                                                     |_|   |_|
    )" << endl;
    int choice = 1;
    while (choice != 0) {
        // testing the tree
        HuffmanTree huffmanTree;
        cout << "Welcome to the Huffman File Zipper!\n";
        cout << "Press 0 to quit\n";
        cout << "Press 1 to zip \n";
        cout << "Press 2 to unzip\n";
        cin >> choice;
        path dir = current_path();
        cout << dir << endl;
        if (choice == 1) {
            cout << "Choose from the following files:\n";
            for(auto entry: directory_iterator(dir) ) {
                cout << entry.path().filename() << "\n";
            }
            string fileName;
            cin >> fileName;
            int oldSize = file_size("./" + fileName + ".txt");
            huffmanTree.Zip(fileName);
            int newSize = file_size("./" + fileName + ".huff");

            cout << "Size before compression: " <<  oldSize << " bytes" << '\n';
            cout << "Size after compression: " <<  newSize << " bytes" << '\n';
            cout << "Memory saved: " << oldSize - newSize << " bytes" << '\n';
        }
        else if (choice == 2) {
            cout << "Choose from the following files:\n";
            for(auto entry: directory_iterator(dir) ) {
                cout << entry.path().filename() << "\n";
            }
            string fileName;
            cin >> fileName;
            huffmanTree.UnZip(fileName);
        }
    }
    return 0;
}
