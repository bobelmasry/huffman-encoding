#include <fstream>
#include "huffmantree.h"
using namespace std;

#include <filesystem>
#include <iostream>

void UI()
{
   int choice = 1;

   while (choice != 0)
   {
      cout << "\033[35m "<< R"(
               __  __                            ___ _ _         ______
  /\  /\_   _ / _|/ _|_ __ ___   __ _ _ __      / __(_) | ___   / _  (_)_ __  _ __   ___ _ __
 / /_/ / | | | |_| |_| '_ ` _ \ / _` | '_ \    / _\ | | |/ _ \  \// /| | '_ \| '_ \ / _ \ '__|
/ __  /| |_| |  _|  _| | | | | | (_| | | | |  / /   | | |  __/   / //\ | |_) | |_) |  __/ |
\/ /_/  \__,_|_| |_| |_| |_| |_|\__,_|_| |_|  \/    |_|_|\___|  /____/_| .__/| .__/ \___|_|
                                                                       |_|   |_|
    )" <<"\033[0m" <<endl;
      // testing the tree
      cout << "\033[36mWelcome to the Huffman File Zipper!" << endl;
      cout << "[1] To Zip a File " << endl;
      cout << "[2] To Unzip a File " << endl;
      cout << "\033[31m[0] Quit\033[36m" << endl;
      cin >> choice;
      filesystem::path dir = filesystem::current_path();
      HuffmanTree huffmanTree;
      if (choice == 1)
      {
         cout << "Files in the Active Directory:\n";
         for (const auto &entry : filesystem::directory_iterator(dir))
         {
            // Check if the entry is a regular file and has a .txt extension
            if (entry.is_regular_file() && entry.path().extension() == ".txt")
            {
               std::cout << entry.path().filename() << "\n";
            }
         }
         cout << "Enter the name of the file (or the Absoloute path): ";
         string fileName;
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         getline(cin, fileName);
         double oldSize, newSize;
         if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt") {
        fileName.erase(fileName.size() - 4); // Remove the last 4 characters
    }
         oldSize = filesystem::file_size(fileName+".txt");
         huffmanTree.Zip(fileName);
         newSize = filesystem::file_size(fileName + ".huff");
         cout << "Compression Analysis:\n";
         cout << "Size before compression: " << oldSize << " bytes" << '\n';
         cout << "Size after compression: " << newSize << " bytes" << '\n';
         cout << "Hiffman efficiency: " << (1-((oldSize - newSize)/oldSize))*100 << "%\033[0m" << '\n';
      }
      else if (choice == 2)
      {
        cout << "Files in the Active Directory:\n";
                 for (const auto &entry : filesystem::directory_iterator(dir))
         {
            // Check if the entry is a regular file and has a .txt extension
            if (entry.is_regular_file() && entry.path().extension() == ".huff")
            {
               std::cout << entry.path().filename() << "\n";
            }
         }
         cout << "Enter the name of the file (or the Absoloute path): ";
         string fileName;
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         getline(cin, fileName);
         huffmanTree.UnZip(fileName);
      }
      else cout << "Unsupported input\n";
   }
}