// staMaker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Loader.h"
#include <map>
#include "GltfLoader.h"

int main()
{

    std::cout << "Hello World!\n";
    std::cout << "Input file name without extension" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << "Converting .. " << std::endl;
    std::string inputGltf = "inputs/" + name + ".gltf";
    std::string inputBin = "inputs/" + name + ".bin";
    std::string outputStc = "outputs/" + name + ".stc";
    convertGLTF(inputGltf, inputBin, outputStc);
    //convertObj(name,outType);
    std::cout << "Done!!" << std::endl;
    //system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
