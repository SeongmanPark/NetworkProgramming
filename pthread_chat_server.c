// pthread를 이용한 멀티 쓰레드 기반 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MAX_CLNT 16

void* handle_clnt(void* arg);
// user가 지정하는 thread 함수
void send_msg(char* msg, int len);
// 서버에 연결된 각 client한테 메세지 보내는 함수
//void errsay(char* msg, int code);

int clnt_cnt = 0;
// 연결된 클라이언트의 개수를 저장하는 변수
int clnt_socks[MAX_CLNT];
// 연결된 클라이언트를 관리하는 배열
pthread_mutex_t mutex;
// mutex 변수

int main(int argc, char** argv)
{
    pthread_mutex_init(&mutex, NULL);
    // 뮤텍스 변수를 초기화, 속성은 NULL 값으로 -> 기본값
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    // 서버 소켓을 생성
    pthread_t t_id;
    // 스레드를 구분하기 위한 스레드 id
    struct sockaddr_in serv_adr, clnt_adr;
    // 서버 소켓 구조체, 클라이언트 소켓 구조체 생성
    socklen_t clnt_adr_sz = sizeof(clnt_adr);
    // accept 함수 마지막 인자에 들어갈 클라이언트 소켓 사이즈
    memset(&serv_adr, 0, sizeof(serv_adr));
    // 서버 소켓 구조체를 0으로 초기화
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    // IPv6, 주소할당, 포트번호 할당
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        perror("bind()");
        exit(1);
    }
    // 소켓 번호와 소켓 주소를 bind

    listen(serv_sock, 5);
    // 대기큐 생성, 최대 5개의 클라이언트

    while(1)
    {
        if(clnt_cnt < MAX_CLNT) // 연결된 클라이언트의 개수가 16개 미만이라면 accept
        {
            int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
            // accept함수를 통해 대기큐 맨 앞에 있는 클라이언트 연결, clnt_sock는 클라이언트 소켓 번호
            pthread_mutex_lock(&mutex);
            // 뮤텍스 시작 !!!!!!! 
            clnt_socks[clnt_cnt] = clnt_sock;
            // clnt_cnt++하기 전에 또 다른 클라이언트가 연결된다면 클라이언트 하나는 서버에 연결 안 되기 때문에
            // 뮤텍스를 걸어준다.
            clnt_cnt++;
            pthread_mutex_unlock(&mutex);

            pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
            // pthread 생성 !!! 스레드 아이디는 t_id에 저장, handler_clnt함수를 실행시키고 이때 함수의 인자는 clnt_sock (클라이언트 소켓 구조체)
            pthread_detach(t_id);
            // pthread_join을 사용하지 않더라도 pthread_detach를 사용하면 쓰레드번호 t_id를 가진 쓰레드가 종료될때 모든 자원을 반납한다. 
            printf("Connect client IP : %s\n", inet_ntoa(clnt_adr.sin_addr));
            // 연결된 클라이언트의 IP주소 출력하기
        }
    }
    pthread_mutex_destroy(&mutex);
}

void* handle_clnt(void* arg)
{
    int clnt_sock = *(int*)arg;
    // void(*)형태로 받은 인자를 int로 타입 캐스팅
    int str_len = 0, i;
    char msg[BUFSIZ];


    while((str_len = read(clnt_sock, msg, BUFSIZ)) != 0)
    {
        send_msg(msg, str_len);
    }

    pthread_mutex_lock(&mutex);
    //클라이언트 배열을 초기화 하는 도중 다른 클라이언트가 임계영역에 들어올수 없음.
    for(i=0; i < clnt_cnt; i++)
    {
        if(clnt_sock == clnt_socks[i]) // 메세지를 보낸 클라이언트를 처리하였기 때문에 클라이언트 배열에서 뺀다. 
        {
            while(i++ < clnt_cnt - 1) // 처리할 클라이언트를 기준으로 배열 초기화 (앞으로 한칸씩 당김)
            {
                clnt_socks[i] = clnt_socks[i+1];
            }
            break;
        }
    }

    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}

void send_msg(char* msg, int len)
{
    int i;
    pthread_mutex_lock(&mutex);

    for(i=0; i < clnt_cnt; i++)
    {
        write(clnt_socks[i], msg, len);
        // 연결된 모든 클라이언트에게 메세지 전달하기
    }
    pthread_mutex_unlock(&mutex);
}
