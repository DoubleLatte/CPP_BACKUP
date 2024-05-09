#include <iostream>
#include <windows.h>
#include <Shlobj.h>
#include <fstream>
#include <string>
#include "ftp.h"


class FolderCreator {
public:
    FolderCreator() {
        // 생성자
        HRESULT result = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &userProfilePath_);

        if (result != S_OK) {
            std::cerr << "Error getting user profile directory!" << std::endl;
            exit(1);
        }
    }

    ~FolderCreator() {
        // 소멸자
        CoTaskMemFree(userProfilePath_);
    }

    void createFolder() {
        wchar_t folderPath[MAX_PATH];
        string filepath = "setting.txt";
        string  slash = "\\";
        swprintf(folderPath, MAX_PATH, L"%s\\%s", userProfilePath_,L"FILE_BACKUP");

        if (!CreateDirectory(folderPath, NULL)) {
            std::cerr << "Error creating directory!" << std::endl;
            exit(1);
        }

        std::wcout << L"Directory created successfully at: " << folderPath << std::endl;
        
        wstring ws(folderPath);
        string s(ws.begin(), ws.end());

        filepath= s + slash + filepath;
        std::ofstream file(filepath);  // "output.txt"라는 이름의 파일을 생성합니다.
        if (file.is_open()) {  // 파일이 성공적으로 열렸는지 확인합니다.
            file << "SeverIp=\nusername=\npasswd=";  // 파일에 "SeverIp"라는 텍스트를 씁니다.
            file.close();  // 파일을 닫습니다.
        }
        else {
            std::cout << "파일을 열 수 없습니다.";
        }
    }

private:
    wchar_t* userProfilePath_;
};

int main() {
    Ftp ftp;
    ftp.ListFiles();
    ftp.UploadFile(L"C:\\Users\\power\\source\\repos\\mainer\\mainer\\ftp.h", L"TIMEs");
    


    FolderCreator folderCreator;
    folderCreator.createFolder();
    return 0;
}
