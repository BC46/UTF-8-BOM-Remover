#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: utf8-bom-remover.exe file-path" << std::endl;
        return 1;
    }

    char bom[4] { -17, -69, -65, '\0' };
    char buffer[4];
    buffer[3] = '\0';

    std::string filePath(argv[1]);
    assert(std::filesystem::exists(filePath));

    std::string newName(filePath + "_tmp");

    rename(filePath.c_str(), newName.c_str());

    std::ifstream fileIn;
    fileIn.open(newName, std::ios::binary | std::ios::out);

    fileIn.read(buffer, 3);

    if (strcmp(buffer, bom) == 0) {
        fileIn.seekg(3);

        std::ofstream fileOut;
        fileOut.open(filePath, std::ios::binary | std::ios::out);

        for (char character; fileIn.get(character);)
            fileOut.write(&character, 1);

        fileIn.close();
        fileOut.close();
        remove(newName.c_str());
    } else {
        fileIn.close();
        rename(newName.c_str(), filePath.c_str());
    }

    return 0;
}
