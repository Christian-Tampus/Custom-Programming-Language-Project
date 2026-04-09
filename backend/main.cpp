/* UPDATE VERSION [10] */

/*
To Execute .cpp (C++) Code You Must Compile First Then Run The Executable (.exe)
Step 1: g++ [PROGRAM_DIRECTORY] -o [NEW_PROGRAM_NAME]
Step 2: ./[PROGRAM_NAME]
Instead Of Doing This Everytime You Can Automate This Build/Run Task With tasks.json
1. Press: Ctrl + Shift + P
2. Type: Tasks: Configure Tasks
3. Choose: Create tasks.json from template -> Others
4. Run Build Tasks: Ctrl + Shift + B (To Execute Code!)
5. Press: Ctrl + Shift + D
6. Click: Create a launch.json file
7. Choose: C++ (Windows)
*/

#include <iostream>

using namespace std;
int main()
{
    std::cout << "[SERVER] main.cpp Program Start!" << std::endl;
    std::cout << "WORKING HERE!" << std::endl;
    std::cout << "[SERVER] main.cpp Program Terminated!" << std::endl;
    return 0;
};