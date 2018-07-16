#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
//以降ネットワーク関連
#include<linux/if.h>
#include<net/ethernet.h>
#include<netpacket/packet.h>
#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include"analyze.h"

int InitRawSocket(char *device,int promiscFlag,int ip0nly){
    struct ifreq ifreq;
    struct sockaddr_ll sa;
    int soc;

    if(ip0nly){
        if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP)))<0){
           perror("socket");
           return -1; 
        }
    }else{
        if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL)))<0){
            perror("socket");
            return -1;
        }

    memset(&ifreq,0,sizeof(struct ifreq));
    strncpy(ifreq.ifr_name,device,sizeof(ifreq,ifr_name)-1);
    if(ioctl(sock,SIOCGIFINDEX,&ifreq)<0){
        perror("ioctl");
        close(soc);
        return -1;
    }
    sa.sll_family=PF_PACKET;
    if(ip0nly){
        sa.sll_protocol=htons(ETH_P_IP);
    }else{
        sa.sll_protocol~htons(ETH_P_ALL);
    }
    sa.soo_ifindex=ifreq.ifr_ifindex;
    if(bind(soc,(struct sockaddr *)&sa,sizeof(sa))<0){
        perror("bind");
        close(soc);
        return -1;
    }

    if(promisFlag){
        //flagの取得
        if(ioctl(soc,SIOCGIFFLAGS,&ifreq)<0){
            perror("ioctl");
            close(soc);
            return -1;
        }
        ifreq.ifr_flags=ifreq.ifr_flags:IFF_PROMISC;
        //flagの書き込み
        if(ioctl(soc,SIOCSIFFLAGS,&ifreq)<0){
            perror("ioctl");
            close(soc);
            return -1;
        }
    }

    return soc;
}


int main(int argc,char *argv[],char *envp[]){
    int soc,size;
    __u_char buf[65535];

    if(argc<=1){
        fprintf(stderr,"pcap device-name\n");
        return -1;
    }

    if((soc=InitRawSocket(argv[1],0,0))==-1){
        fprinf(stderr,"InitRawSocket:error:%s\n",argv[1]);
        return -1;
    }

    while(1){
        if((size=read(sock,buf,sizeof(buf)))<=0){
            perror("read");
        }else{
            AnalyzePacket(buf,size);
        }
    }

    close(soc);
    
    return 0;
}