#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<linux/if.h>
#include<net/ethernet.h>
#include<netpacket/packet.h>
#include<netinet/if_ether.h>

extern int DebugPrintf(char *fmt,...);
extern int DebugPerror(char *msg);
 
 int InitRawSocket(char *device,int promiscFlag,int ip0nly){
    struct ifreq ifreq;
    struct sockaddr_ll sa;
    int soc;

    if(ip0nly){
        if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP)))<0){
            DebugPerror("socket");
            return -1;
        }else{
            if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL)))<0){
                DebugPerror("socket");
                return -1;
            }
        }
    }

    memset(&ifreq,0,sizeof(struct ifreq));
    strncpy(ifreq.ifr_name,device,sizeof(ifreq.ifr_name)-1);
    if(ioctl(soc,SIOCGIFINDEX,&ifreq)<0){
        DebugPerror("ioctl");
        close(soc);
        return -1;
    }
    sa.sll_family=PF_PACKET;
    if(ip0nly){
        sa.sll_protocol=htons(ETH_P_IP);
    }else{
        sa.sll_protocol=htons(ETH_P_ALL);
    }
    if(bind(soc,(struct sockaddr *)&sa,sizeof(sa))<0){
        DebufPerror("bind");
        close(soc);
        return -1;
    }

    if(promiscFlag){
        if(ioctl(soc,SIOCGIFFLAGS,&ifreq)<0){
            DebugPerror("ioctl");
            close(soc);
            return -1;
        }

        ifreq.ifr_flags = ifreq.ifr_flags|IFF_PROMISC;
        if(ioctl(soc,SIOCGIFFLAGS,&ifreq)<0){
            DebugPerror("ioctl");
            close(soc);
            return -1;
    }

    return soc;

 }
