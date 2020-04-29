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
    struct sockaddr_in serv_adr, clnt_adr;
    // 서버 클라이언트 소켓 구조체
    socklen_t str_len;
    // recvform의 리턴값을 받기위한 변수 (몇 바이트 받았는지 ?)
    int serv_sock;
    // 서버에서 사용할 socket descriptor
    char message[BUF_SIZE];

    serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    // 소켓 생성, serv_sock -> 소켓 디스크립터 번호
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    // serv_adr 서버 소켓 구조체에 주소 할당, 포트번호 할당.

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        perror("bind() error");
        exit(1);
    }
    // bind()함.

    while(1)
    {
        socklen_t clnt_adr_sz;
        // 클라이언트 소켓 구조체의 크기를 나타내는 변수
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        // socket에 있는 내용을 message에 쓴다. 이때 리턴값은 읽은 바이트의 수, 어디서 왔는지 기록하기 위해 클라이언트 소켓 구조체에 저장.
        printf("From Client's IP Address : %s, Port Number : %d\n", inet_ntoa(clnt_adr.sin_addr), clnt_adr.sin_port);
        // 데이터를 보낸 클라이언트의 IP주소 (네트워크주소 빅엔디안 -> Decimal Dot), 포트 번호 출력
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, sizeof(clnt_adr));
        // 다시 그 내용을 클라이언트에게 전송 (echo server)
    }

    close(serv_sock);
    return 0;

}
