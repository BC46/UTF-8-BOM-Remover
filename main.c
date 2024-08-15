#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

inline BOOL fileExists (char* name) {
    return ( _access( name, 0 ) != ENOENT );
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: utf8-bom-remover.exe file-path");
        return 0;
    }

    char bom[4] = { '\xEF', '\xBB', '\xBF', '\0' };
    char buffer[4] = { 0 };

    char* filePath = argv[1];

    if (fileExists(filePath) == FALSE)
        return 0;

    char newName[MAX_PATH] = { 0 };
    strcpy_s(newName, MAX_PATH, argv[1]);
    strcat_s(newName, MAX_PATH, "_tmp");

    rename(filePath, newName);

    FILE* fileIn = fopen(newName, "r");
    fread(buffer, sizeof(char), 3, fileIn);

    if (strcmp(buffer, bom) == 0) {
        fseek(fileIn, 3, SEEK_SET);

        FILE* fileOut = fopen(filePath, "w");

        char c = fgetc(fileIn);

        while (c != EOF) {
            fputc(c, fileOut);
            c = fgetc(fileIn);
        }

        fclose(fileIn);
        fclose(fileOut);
        remove(newName);
    } else {
        fclose(fileIn);
        rename(newName, filePath);
    }

    return 0;
}
