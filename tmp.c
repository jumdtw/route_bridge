#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
//以降ネットワークインターフェイス関係
#include<linux/if.h>
#include<net/ethernet.h>
#include<netpacket/packet.h>
#include<netinet/if_ether.h>

int main(){
    int a = htons(ETH_P_ALL);
    printf("%d",a);
    return 0;
}