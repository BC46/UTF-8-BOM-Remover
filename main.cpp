#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>

int main() {
    // TODO: Take arguments

    // TODO: Change bom to string
    char bom[4] { -17, -69, -65, NULL };
    char buffer[4];
    buffer[3] = NULL;

    std::ifstream fileIn;
    std::ofstream fileOut;

    // TODO do not use hardcoded path
    assert(std::filesystem::exists("C:\\test1.txt"));

    std::string newName(std::string("C:\\test1.txt") + "_tmp");

    // TODO do not use hardcoded path
    rename(std::string("C:\\test1.txt").c_str(), newName.c_str());
    fileIn.open(newName, std::ios::binary | std::ios::out);

    fileOut.open("C:\\test1.txt", std::ios::binary | std::ios::out);

    fileIn.read(buffer, 3);

    if (strcmp(buffer, bom) == 0) {
        std::cout << "BOM found!" << std::endl;
        fileIn.seekg(3);

        for (char character; fileIn.get(character);) {
            fileOut.write(&character, 1);
        }
    }

    fileIn.close();
    fileOut.close();

    remove(newName.c_str());

    return 0;
}
