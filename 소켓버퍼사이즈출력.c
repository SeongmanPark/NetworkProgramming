#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>

int main(int argc, char** argv)
{
    int z;
    // 함수 호출 후 에러를 확인하기 위한 변수.
    int s = -1; // socket descriptor
    int sndbuf, rcvbuf;
    // socket send , recv buffer size
    socklen_t optlen; // option length

    s = socket(PF_INET, SOCK_STREAM, 0);
    // create socket
    if(s == -1)
    {
        perror("socket(2)");
        exit(1);
    }

    optlen = sizeof(sndbuf);
    // option length
    z = getsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbuf, &optlen);
    // getsockopt(sockfd, int level, int optname, void* optval, socklen_t optlen)
    if(z == -1)
    {
        perror("getsockopt(2)");
        exit(1);
    }

    optlen = sizeof(rcvbuf);
    z = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen);
    if(z == -1)
    {
        perror("getsockopt(2)");
        exit(1);
    }
    printf(" Socket s : %d\n", s);
    printf(" Send buf : %d bytes\n", sndbuf);
    printf(" Recv buf : %d bytes\n", rcvbuf);
    close(s);
    return 0;

}
