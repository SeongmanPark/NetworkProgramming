// pthread를 이용한 멀티 쓰레드 기반 클라이언트

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void* send_msg(void* arg);
void* recv_msg(void* arg);
void* errsay(char* msg, int code);
char name[BUFSIZ];
char msg[BUFSIZ];

int main(int argc, char** argv)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    // 소켓 생성
    pthread_t snd_thread, rcv_thread;
    // 수신, 송신 스레드 id 설정
    void* pthread_return;
    // 스레드 상태를 저장하는 변수 저장 (pthread_join에 쓰임)
    struct sockaddr_in serv_adr;
    // 서버와 연결할 구조체 생성

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) // 서버와 연결
    {
        perror("connect() error");
        exit(1);
    }

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    // 수신 스레드 생성
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    // 송신 스레드 생성
    pthread_join(snd_thread, &pthread_return);
    pthread_join(rcv_thread, &pthread_return);
    // 두 개(수신, 송신)의 스레드가 종료되기를 기다리는 함수
    close(sock);
    return 0;
}

void* send_msg(void* arg)
{
    int sock = *((int*)arg);
    // 서버 소켓 번호 인자로 받아오기
    char name_msg[BUFSIZ];
    while(1)
    {
        fgets(msg, BUFSIZ, stdin);
        // 표준 입력에 입력하여 msg 배열에 넣기
        if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }

    return NULL;
}

void* recv_msg(void* arg)
{
    int sock = *((int*)arg);
    // 서버 소켓 번호 인자로 받아오기
    char name_msg[BUFSIZ];
    int str_len;
    while(1)
    {
        str_len = read(sock, name_msg, BUFSIZ); // 서버 소켓에 있는 데이터를 읽음, 이때 리턴값은 읽은 데이터의 길이
        if(str_len == -1)
        {
            return (void*)-1;
        }
        name_msg[str_len] = '\0';
        fputs(name_msg, stdout);
        // 표준 출력에 출력 !!!!
    }
    return NULL;
}
