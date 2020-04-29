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
    int so_type = -1; //socket type
    socklen_t optlen; // option length

    s = socket(PF_INET, SOCK_STREAM, 0);
    // create socket
    if(s == -1)
    {
        perror("socket(2)");
        exit(1);
    }

    optlen = sizeof(so_type);
    // option length
    z = getsockopt(s, SOL_SOCKET, SO_TYPE, &so_type, &optlen);
    // getsockopt(sockfd, int level, int optname, void* optval, socklen_t optlen)
    if(z == -1)
    {
        perror("getsockopt(2)");
        exit(1);
    }

    printf(" Socket s : %d\n", s);
    printf(" SO_TYPE : %d\n", so_type);
    // TCP STREAM 은 SO_TYPE가 1
    // UDP DGRAM 은 SO_TYPE이 2
    printf(" SOCK_STREAM = %d\n", SOCK_STREAM);
    close(s);
    return 0;

}
