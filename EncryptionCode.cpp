#include <iostream>
#include <fstream>
using namespace std;
// READY TO BE USED
void encrypt(const int key) {
   ifstream inputFile("Input.txt"); 
   ofstream outFile("Encrypted.txt");
   char ch;
    while (inputFile.get(ch)){
        outFile <<  char(ch+key%255) ;} 

    outFile.close();      
    inputFile.close();
   
}
void decrypt(const int key) {
   ifstream inputFile("Encrypted.txt");
   char ch;
   while (inputFile.get(ch))
         cout << char(ch-key%255);
}

int main() {   
   int key =69;  
   encrypt(key);
   decrypt(key);
   return 0;
    }
