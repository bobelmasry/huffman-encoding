# Huffman File Zipper

### **Team Members**
- Youssef Hawash
- Moaz Eldamery
- Nour Elghaly
- Jacques Jean
- Aly Hassan

---

## **Project Overview**
The **Huffman File Zipper** is a file compression utility built to efficiently compress and decompress files using the Huffman coding algorithm. 

## **Features**
- **Compression**: Compresses files to save disk space.
- **Decompression**: Restores compressed files to their original state.
- **Huffman Coding**: Implements the Huffman algorithm to provide efficient lossless compression.
- **Command-Line Interface**: Easy to use command-line interactions.
- **Graphical User Interface**: Easy to use Graphical Interface.
- **Error Handling**: Ensures integrity and consistency during compression and decompression processes.


## **Project Structure**
```
Huffman-File-Zipper/
├── .vscode/
│   └── settings.json             # VSCode settings for the project
├── Additional Features/
│   └── EncryptionCode.cpp        # Additional encryption features
├── GUI_bonus/
│   └── file_zipper/
│       ├── file_zipper.pro       # Project configuration file for GUI
│       ├── file_zipper.pro.user  # User-specific configuration
│       ├── huffmantree.cpp       # Huffman tree implementation
│       ├── huffmantree.h         # Huffman tree header
│       ├── main.cpp              # Main file for GUI implementation
│       ├── mainwindow.cpp        # GUI main window implementation
│       ├── mainwindow.h          # GUI main window header
│       ├── mainwindow.ui         # GUI layout file
│       ├── priorityqueue.cpp     # Priority queue implementation
│       └── priorityqueue.h       # Priority queue header
├── lib/
│   └── priorityqueue.cpp         # Priority queue library implementation
│   └── priorityqueue.h           # Priority queue library header
├── Tests/
│   ├── CMakeLists.txt            # CMake configuration for tests
│   ├── file_comparator_test.cpp  # Unit tests for file comparator
├── .gitignore                    # Git ignore file
├── CMakeLists.txt                # CMake configuration file
├── huffmantree.cpp               # Huffman tree implementation
├── huffmantree.h                 # Huffman tree header
├── main.cpp                      # Main program file
├── README.md                     # Project documentation
└── UI.cpp                        # User Interface implementation
```