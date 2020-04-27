// socket 통신 (서버)
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
    int serv_sock, clnt_sock;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    // 소켓(Internet) 구조체 선언
    socklen_t clnt_addr_size;
    // 소켓의 구조체의 크기를 나타내는 변수
    char message[] = "Hello World!";
    // 메세지
    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    // 서버에서 사용할 소켓 생성 (ipv4, tcp), serv_sock는 socket descriptor
    if(serv_sock == -1)
    {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 서버에서 사용할 소켓 구조체 초기화
    serv_addr.sin_family = AF_INET;
    // 표준 ipv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // ip주소 지정
    serv_addr.sin_port = htons(atoi(argv[1]));
    // port 번호 지정
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error");
    }
    // 생성한 socket을 server socket으로 등록
    if(listen(serv_sock, 5) == -1)
    {
        error_handling("listen() error");
    }
    // server socket을 통해 클라이언트의 접속 요청을 확인하도록 waiting queue 생성, 크기 5
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    // client 접속 요청에 따라 accept()로 접속을 허락.
    // 이제 client socket까지 만들어 졌으므로 socket descriptor를 통해 write, read 가능
    if(clnt_sock == -1)
    {
        error_handling("accept() error");
    }
    write(clnt_sock, message, sizeof(message));
    // clnt_sock에 메세지 쓰기
    close(clnt_sock);
    close(serv_sock);
    // socket descriptor 닫기
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}