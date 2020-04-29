#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 30

int main(int argc, char** argv)
{
    int sock;
    socklen_t str_len;
    struct sockaddr_in serv_adr, from_adr;
    char message[BUF_SIZE];

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    // 통신할 소켓 생성
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    // 연결할 서버의 IP주소와 Port 번호를 구조체에 등록한다.

    while(1)
    {
        socklen_t from_adr_sz;
        from_adr_sz = sizeof(serv_adr);
        scanf("%s", message);
        // 서버에게 보낼 메세지 입력
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        // 서버에게 메시지를 보낸다
        str_len = recvfrom(sock, message, strlen(message), 0, (struct sockaddr*)&from_adr, &from_adr_sz);
        // 서버로부터 echo 메시지를 받는다. 이때 from_adr구조체와 serv_adr 구조체는 같은 것이다.
        message[str_len] = '\0';
        // 메시지의 마지막은 '\0'
        printf("Message from server : %s\n", message);
        // 서버로부터 받은 메시지 출력
        printf("From Server's IP Address : %s, Port Number : %d\n", inet_ntoa(from_adr.sin_addr), from_adr.sin_port);
        // 서버의 IP주소와 Port Number 출력
    }
    return 0;
}
