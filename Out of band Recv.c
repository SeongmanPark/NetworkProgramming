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
int acpt_sock;
int recv_sock;

void urg_handler(int signo) // out of band data 받았을때 발생하는 handler
{
    int readn;
    char buf[BUF_SIZE];
    printf("SIGURG Receive!\n");
    memset(buf, 0x00, sizeof(buf));
    readn = recv(recv_sock, buf, sizeof(buf), MSG_OOB);
    //printf("readn : %d \n", readn);
    printf("URG DATA %s\n", buf);
}

int main(int argc, char** argv)
{
    struct sockaddr_in recv_adr, serv_adr;
    // recv_adr : 클라이언트 소켓
    // serv_adr : 서버의 소켓
    int str_len, state;
    socklen_t recv_adr_sz;
    struct sigaction act;
    // sigaction 구조체
    char buf[BUF_SIZE];

    act.sa_handler = urg_handler;
    // handler 지정하기
    sigemptyset(&act.sa_mask);
    // sa_mask를 0으로 초기화 !!!! 모든 시그널에 반응한다.
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    // 서버의 소켓 등록하기
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 커널이 자동으로 서버 주소 할당해줌.
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        perror("bind(2)");
        exit(1);
    }
    // bind() 서버의 IP주소와 port번호 할당
    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    // 클라이언트 소켓 구조체의 크기
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);
    // 대기큐에 있는 클라이언트 소켓과 연결 -> recv_sock
    fcntl(recv_sock, F_SETOWN, getpid());
    // 비동기 입출력과 관련되어서 사용되며, SIGIO, SIGURG 시그널을 수신하는 프로세스 아이디(혹은 그룹)을 설정하기 위해서 사용된다.
    state = sigaction(SIGURG, &act, 0);
    // 핸들러 등록한다 !!!

    while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) // 클라이언트 소켓에 있는 내용을 buf에 쓴다, 이때 리턴값은 실제로 수신한 데이터의 길이
    {
        //printf("str_len : %d\n", str_len);
        // SIGURG를 수신하면 핸들러에서 버퍼를 처리해주고 str_len은 -1이 되기 때문에 while문 안의 연산자는 != 0 이 맞다.
        if(str_len == -1)
            continue;

        buf[str_len] = '\0';
        // 읽은 문자열의 마지막을 \0으로
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);

    return 0;
}
