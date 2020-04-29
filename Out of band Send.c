#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>

#define BUF_SIZE 30

int main(int arvc, char** argv)
{
    int sock;
    struct sockaddr_in recv_adr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
    {
        perror("connect error");
        exit(1);
    }
    // 서버에 연결을 요청함, socket descriptor로 통신
    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    // URG DATA 0
    // 89
    // 가 출력되는 이유는 시그널 SIGURG를 받았을 때 버퍼의 맨 마지막을 출력한다고 함.

    sleep(1);
    close(sock);
    return 0;
}
