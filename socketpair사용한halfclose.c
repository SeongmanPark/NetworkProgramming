// parent, child 사이 socketpair
// socketpair를 이용한 IPC
// 두 소켓을 연결하기 위한 별도의 과정이 필요 없다.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    int z, s[2];
    char* msgp, buf[80];
    pid_t chpid;

    z = socketpair(AF_LOCAL, SOCK_STREAM, 0, s);
    // local에서 데이터를 주고받을때, (AF_UNIX or AF_LOCAL, TCP, 프로토콜, fd)
    // socket에 연결된 s[2] 
    if(z < 0)
    {
        perror("socketpair");
        exit(1);
    }

    if((chpid = fork()) == (pid_t)-1)
    {
        perror("fork");
        exit(1);
    }
    
    else if(chpid == 0) // child process (자식 프로세스)
    {
        char rxbuf[80];
        printf("Parent PID : %d\n", getppid());
        close(s[0]);
        // child는 s[1]을 가지고 통신할 예정
        s[0] = -1;
        msgp = "%A %d-%b-%Y %l:%M %p";
        printf("Sending Message : %s\n", msgp);
        fflush(stdout);

        z = write(s[1], msgp, strlen(msgp));
        // 소켓에다가 문자열 msgp에 담긴 내용을 쓴다.
        if(z<0)
        {
            perror("write");
            exit(1);
        }

        if(shutdown(s[1], SHUT_WR) == -1) // 다 썼으니 출력스트림 close (half close)
        {
            perror("shutdown");
            exit(1);
        }

        z = read(s[1], rxbuf, sizeof(rxbuf));
        // 소켓에다가 쓴 내용을 rxbuf에 저장한다.
        // z byte 읽음.
        if(z<0)
        {
            perror("read");
            exit(1);
        }
        rxbuf[z] = '\0';
        // EOF을 가리키기 위해 \0 삽입.
        printf("Recevied Message : %s\n", rxbuf);
        // rxbuf 출력
        fflush(stdout);
        close(s[1]);
    }

    else // parent process (부모 프로세스)
    {
        int status;
        char txbuf[80];
        time_t td;
        printf("Child pid : %d\n", chpid);
        fflush(stdout);
        close(s[1]);
        // parent는 s[0]으로 통신
        s[1] = -1;
        
        z = read(s[0], buf, sizeof(buf));
        // 소켓에 있는 데이터를 읽어서 buf에 저장.
        // 아마 child 부분의 "%A %d-%b-%Y %l:%M %p" 읽었을 것임.
        // z -> 읽은 byte 수
        if(z < 0)
        {
            perror("read");
            exit(1);
        }
        buf[z] = '\0';
        time(&td);
        strftime(txbuf, sizeof(txbuf), buf, localtime(&td));
        // buf으로 가리키는 스트링에서 제어하는 대로, txbuf에 바이트를 배치
        z = write(s[0], txbuf, strlen(txbuf));
        // txbuf의 내용을 소켓에다가 쓰기.. (숫자가 입력된 요일, 날짜, 시간)
        if(z<0)
        {
            perror("write");
            exit(1);
        }
        close(s[0]);
        waitpid(chpid, &status, 0);
        // 자식 기다리기.
        return 0;
    }
    return 0;
}
