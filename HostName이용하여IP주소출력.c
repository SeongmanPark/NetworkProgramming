// hostname을 가지고 ip주소값 출력

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    struct hostent* myent;
    myent = gethostbyname(argv[1]);
    // 주어진 호스트 argv[1]에 상응하는 hostent 구조체 반환.

    printf("Host Name : %s\n", myent->h_name);
    // 공식 Domain Name 출력
    for(int i=0; myent->h_aliases[i]; i++)
    {
        printf("Alias %d : %s\n", i+1, myent->h_aliases[i]);
        // 공식 Domain Name 이외의 Domain Name 출력하기
    }

    printf("Address Type : %s\n", (myent->h_addrtype == AF_INET)?"AF_INET":"AF_INET6");
    // 주소정보 체계 출력하기 IPv4 인지 ? IPv6인지 ??
    /*for( ; *myent->h_addr_list != NULL; myent->h_addr_list++)
    {
        printf("Address %d : %s\n", ++i, inet_ntoa(*(struct in_addr*)*myent->h_addr_list));
    }*/
    for(int i=0 ; myent->h_addr_list[i]; i++)
    {
        printf("Address %d : %s\n", i+1, inet_ntoa(*(struct in_addr*)myent->h_addr_list[i]));
        // Domain Name에 대한 IP주소 출력하기
        // inet_ntoa의 인자가 struct in_addr 구조체이므로
        // 1. (struct in_addr*) 포인터로 캐스팅 변환을 해 준다.
        // 2. 캐스팅 변환을 하면 (struct in_addr*) 이므로 *를 써주어서 형 변환을 맞춰준다. (struct in_addr)로
    }
}
