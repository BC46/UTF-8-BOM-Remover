#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

BOOL canAccessFile(wchar_t* path) {
    return _waccess(path, 0) == 0 ? TRUE : FALSE;
}

int wmain(int argc, wchar_t *argv[]) {
    char bom[4] = { '\xEF', '\xBB', '\xBF', '\0' };
    char buffer[4] = { 0 };
    wchar_t* filePath;
    wchar_t newName[MAX_PATH];

    char c;

    FILE* fileIn;
    FILE* fileOut;

    if (argc < 2) {
        puts("Usage: utf8-bom-remover.exe file-path");
        return 0;
    }

    filePath = argv[1];

    if (canAccessFile(filePath) == FALSE)
        return 0;

    ZeroMemory(newName, MAX_PATH * sizeof(wchar_t));
    wcscpy(newName, filePath);
    wcscpy(newName, L"_tmp");

    _wrename(filePath, newName);

    fileIn = _wfopen(newName, L"r");
    fread(buffer, sizeof(char), 3, fileIn);

    if (strcmp(buffer, bom) == 0) {
        fseek(fileIn, 3, SEEK_SET);

        fileOut = _wfopen(filePath, L"w");

        c = fgetc(fileIn);

        while (c != EOF) {
            fputc(c, fileOut);
            c = fgetc(fileIn);
        }

        fclose(fileIn);
        fclose(fileOut);
        _wremove(newName);
    } else {
        fclose(fileIn);
        _wrename(newName, filePath);
    }

    return 0;
}
