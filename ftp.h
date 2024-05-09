#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#pragma comment(lib, "wininet.lib")

using namespace std;


class Ftp
{
public:
    Ftp();

    void ListFiles() {
        WIN32_FIND_DATAW findFileData;
        HINTERNET hFind = FtpFindFirstFileW(hConnect, NULL, &findFileData, INTERNET_FLAG_RELOAD, 0);

        if (hFind == NULL) {
            wcout << L"오류: " << GetLastError() << endl;
            return;
        }

        do {
            wcout << findFileData.cFileName << endl;
        } while (InternetFindNextFileW(hFind, &findFileData));

        InternetCloseHandle(hFind);
    }

    void UploadFile(LPCWSTR localFile, LPCWSTR remoteFile) {
        if (!FtpPutFile(hConnect, localFile, remoteFile, FTP_TRANSFER_TYPE_BINARY, 0)) {
            cout << "Upload failed" << endl;
        }
        else
        {
            cout << "Upload ok" << endl;
        }
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
    }
    void DownloadFile(LPCWSTR remoteFile, LPCWSTR localFile) {
        if (!FtpGetFile(hConnect, remoteFile, localFile, FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0)) {
            cout << "Download failed \n Ftp 서버의 권한을 확인해 주세요!" << endl;
        }
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
    }
    bool readSettingsFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "파일을 열 수 없습니다." << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                if (key == "severip") {
                    serverAddress = wstring(value.begin(), value.end()); // ASCII 문자열을 유니코드로 변환
                } else if (key == "username") {
                    username = wstring(value.begin(), value.end());
                } else if (key == "password") {
                    password = wstring(value.begin(), value.end());
                }
            }
        }

        file.close();
        return true;
    }

private:
    HINTERNET hInternet, hConnect;
    wstring serverAddress;
    wstring username;
    wstring password;
};

Ftp::Ftp()
{

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        cout << "인터넷에 연결되어 있지 않습니다!" << endl;
    }
    // 파일에서 서버 주소, 사용자 이름 및 비밀번호 읽어오기
    if (!readSettingsFromFile("C:\\Users\\power\\FILE_BACKUP\\setting.txt")) {
        cout << "파일에서 설정을 읽어오지 못했습니다." << endl;
    }
    _Notnull_ hConnect = InternetConnect(hInternet, serverAddress.c_str(), 25560, username.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
    if (hConnect == NULL) {
        cout << "서버와 연결이 되지 않습니다 설정을 다시 확인해주세요!" << endl;
    }
}

