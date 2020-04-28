#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
int main(int argc, char** argv)
{
    struct in_addr inaddr; // 32bit IP구조체
    struct sockaddr_in6 ipv6_addr; // IPv6 소켓 구조체
    char buf[20];
    int sockfd;

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    // 소켓 생성 IPv6 !!!
    ipv6_addr.sin6_family = AF_INET6;
    ipv6_addr.sin6_flowinfo = 0;
    ipv6_addr.sin6_port = htons(8080);
    // Port Number : 8080

    connect(sockfd, (struct sockaddr*)&ipv6_addr, sizeof(ipv6_addr));

    printf("입력 IP : %s\n", argv[1]);
    inet_pton(AF_INET, argv[1], &inaddr.s_addr);
    // argv[1]에 해당하는 문자열 인터넷 주소(10진수)를 binary text로 변경하는 함수
    // argv[1] = 빅 엔디안, &inaddr.s_addr = 리틀 엔디안
    // inet_pton() 문자열을 프로토콜(IPV4,IPV6 등)에 해당하는 네트워크데이터 binary text로 
    printf("inet_pton(%s) = 0x%x\n", argv[1], inaddr.s_addr);

    inet_ntop(AF_INET, &inaddr.s_addr, buf, sizeof(buf));
    // binary text를 문자열 인터넷 주소(10진수)로 변환.
    printf("inet_ntop(0x%x) = %s\n", inaddr.s_addr, buf);

    return 0;
}
