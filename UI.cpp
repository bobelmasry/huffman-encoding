#include <fstream>
#include "huffmantree.h"
#include <limits>
#include <iostream>
using namespace std;

void UI()
{
   int choice = 1;

   while (choice != 0)
   {
      cout << "\033[35m " << R"(
               __  __                            ___ _ _         ______
  /\  /\_   _ / _|/ _|_ __ ___   __ _ _ __      / __(_) | ___   / _  (_)_ __  _ __   ___ _ __
 / /_/ / | | | |_| |_| '_ ` _ \ / _` | '_ \    / _\ | | |/ _ \  \// /| | '_ \| '_ \ / _ \ '__|
/ __  /| |_| |  _|  _| | | | | | (_| | | | |  / /   | | |  __/   / //\ | |_) | |_) |  __/ |
\/ /_/  \__,_|_| |_| |_| |_| |_|\__,_|_| |_|  \/    |_|_|\___|  /____/_| .__/| .__/ \___|_|
                                                                       |_|   |_|
    )" << "\033[0m"
           << endl;
      // testing the tree
      cout << "\033[36mWelcome to the Huffman File Zipper!" << endl;
      cout << "[1] To Zip a File " << endl;
      cout << "[2] To Unzip a File " << endl;
      cout << "\033[31m[0] Quit\033[36m" << endl;
      cin >> choice;
      HuffmanTree huffmanTree;
      if (choice == 1)
      {
         cout << "Files in the Active Directory:\n";
         cout << "Enter the name of the file (or the Absoloute path): ";
         string fileName;
         cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         getline(cin, fileName);
         double oldSize, newSize;
         if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt")
         {
            fileName.erase(fileName.size() - 4); // Remove the last 4 characters
         }
         oldSize = huffmanTree.getFileSize(fileName + ".txt");
         huffmanTree.Zip(fileName);
         newSize = huffmanTree.getFileSize(fileName + ".huff");
         cout << "Compression Analysis:\n";
         cout << "Size before compression: " << oldSize << " bytes" << '\n';
         cout << "Size after compression: " << newSize << " bytes" << '\n';
         cout << "Huffman efficiency: " << ( ((oldSize - newSize) / oldSize)) * 100 << "%\033[0m" << '\n';
      }
      else if (choice == 2)
      {
         cout << "Files in the Active Directory:\n";
         cout << "Enter the name of the file (or the Absoloute path): ";
         string fileName;
         cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         getline(cin, fileName);
         huffmanTree.UnZip(fileName);
      }
      else if (choice != 0)
         cout << "Unsupported input\n";
   }
}