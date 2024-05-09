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
            wcout << L"����: " << GetLastError() << endl;
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
            cout << "Download failed \n Ftp ������ ������ Ȯ���� �ּ���!" << endl;
        }
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
    }
    bool readSettingsFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "������ �� �� �����ϴ�." << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                if (key == "severip") {
                    serverAddress = wstring(value.begin(), value.end()); // ASCII ���ڿ��� �����ڵ�� ��ȯ
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
        cout << "���ͳݿ� ����Ǿ� ���� �ʽ��ϴ�!" << endl;
    }
    // ���Ͽ��� ���� �ּ�, ����� �̸� �� ��й�ȣ �о����
    if (!readSettingsFromFile("C:\\Users\\power\\FILE_BACKUP\\setting.txt")) {
        cout << "���Ͽ��� ������ �о���� ���߽��ϴ�." << endl;
    }
    _Notnull_ hConnect = InternetConnect(hInternet, serverAddress.c_str(), 25560, username.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
    if (hConnect == NULL) {
        cout << "������ ������ ���� �ʽ��ϴ� ������ �ٽ� Ȯ�����ּ���!" << endl;
    }
}

