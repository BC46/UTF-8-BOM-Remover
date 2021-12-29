#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: utf8-bom-remover.exe file-path" << std::endl;
        return 1;
    }

    char bom[4] { -17, -69, -65, NULL };
    char buffer[4];
    buffer[3] = NULL;

    std::ifstream fileIn;

    std::string filePath(argv[1]);
    assert(std::filesystem::exists(filePath));

    std::string newName(std::string(filePath) + "_tmp");

    rename(std::string(filePath).c_str(), newName.c_str());
    fileIn.open(newName, std::ios::binary | std::ios::out);

    fileIn.read(buffer, 3);

    if (strcmp(buffer, bom) == 0) {
        fileIn.seekg(3);

        std::ofstream fileOut;
        fileOut.open(filePath, std::ios::binary | std::ios::out);

        for (char character; fileIn.get(character);) {
            fileOut.write(&character, 1);
        }

        fileIn.close();
        fileOut.close();
        remove(newName.c_str());
    } else {
        fileIn.close();
        rename(newName.c_str(), std::string(filePath).c_str());
    }

    return 0;
}
