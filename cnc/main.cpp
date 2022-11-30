#include <cstdio>
#include <cstring>
#include <thread>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX 8192

class server{
private:
    int port_number;
    int src_socket;

    WSADATA wsa_data;

    char recv_buf[MAX];
    char send_buf[MAX];

    int status;
    int result;

    char input[MAX];
public:
    server()
    {
        init(1919);
    }

    void init(int n)
    {
        recv_buf[MAX] = {};
        send_buf[MAX] = {};
        port_number = n;
    }

    void cnc();

    ~server() {
        WSACleanup();
    }
};

void server::cnc() {
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
        return;
    }

    struct sockaddr_in src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_port = htons(port_number);
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    src_socket = socket(AF_INET, SOCK_STREAM, 0);

    bind(src_socket, (struct sockaddr *) &src_addr, sizeof(src_addr));

    int dst_socket;
    struct sockaddr_in dst_addr;
    int dst_addr_size = sizeof(dst_addr);

    listen(src_socket, 1);

    while (1) {
        dst_socket = accept(src_socket, (struct sockaddr *) &dst_addr, &dst_addr_size);

        while (1) {
            fgets(input, sizeof(input), stdin);

            send(dst_socket, send_buf, sizeof(send_buf), 0);

            result = recv(dst_socket, recv_buf, sizeof(recv_buf), 0);
            if (result == 0 || result == -1) {
                status = closesocket(dst_socket);
                break;
            }

            printf("%s", recv_buf);
        }
    }
}

int main() {
    server* cnc_server;

    std::thread th(server::cnc, cnc_server);
    th.join();

    return 0;
}