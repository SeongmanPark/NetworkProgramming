// socket 통신 (클라이언트)
// client : destination 파일에다가 소켓에서부터 읽은 데이터를 쓴다.
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char*);

int main(int argc, char** argv)
{
    int sock, wfd;

    struct sockaddr_in serv_addr;
    // 서버 소켓(Internet) 구조체 선언
    char message[BUFSIZ];
    // 서버로부터 읽을 메세지를 저장하는 문자열
    int str_len=0;
    int idx=0, read_len=0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    // 통신에 사용할 소켓 생성 (ipv4, tcp), sock는 socket descriptor
    if(sock == -1)
    {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 서버에서 사용할 소켓 구조체 초기화
    serv_addr.sin_family = AF_INET;
    // 표준 ipv4
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // 서버의 ip주소 지정
    serv_addr.sin_port = htons(atoi(argv[2]));
    // 서버의 port 번호 지정
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }
    // 생성한 socket을 가지고 connect(), 서버에 요청
    wfd = open("b.txt", O_CREAT | O_WRONLY , 0644);

    while((read_len = read(sock, message, sizeof(message))) > 0)
    {
        printf("%d\n\n\n", read_len);
        write(wfd, message, read_len);
        str_len += read_len;
    }
    // 소켓으로 부터 읽은 데이터를 b.txt에 쓰기
    
    close(sock);
    // socket descriptor 닫기
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
