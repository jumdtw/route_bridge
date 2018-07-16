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

int InitRawSocket(char *device,int promiscFlag,int ip0nly){
    
     /*
    以下において下記URLが参考になった。
    http://neineigh.hatenablog.com/entry/2013/09/28/185053

     ifreqの書式は
        #include <sys/ioctl.h>
        #include <net/if.h> 
    である 

    sockaddr_ll
    https://linuxjm.osdn.jp/html/LDP_man-pages/man7/packet.7.html
    
     ifreqはioctlに渡してネットワークデバイスを設定するための構造体らしい。

     sockaddr_llは上記URLに
     sockaddr_ll はデバイスに依存しない物理層のアドレスである。 
    と書かれていた。多分依存状態に関係ないものを構造体でまとめたのだと思う

    　socket()の第一引数tcp,udpではPF_INET,PF_INET6を指定するがデータリンク層を扱う場合PF_PACKETを指定

    　第二引数ではtcpではSOCK_STREAM、udpではSOCK_DGRAMを指定。 データリンク層ではSOCK_RAWを指定する。

    　第三引数では、ETH_P_IPでipのみ、ETH_P_ALLでは全パケットを取得する。

    　htons()がよくわからんETH_P_IPが0x0800,ETH_P_ALLが0x0003であった。第三引数はos依存のようなのでOSが使用しているメモリ領域に
    protocol関係のなんかがあるっぽい。

    　memset()は第一引数で指定したポインタに第二引数で指定した値を第三引数を数値分書き込むらしい
    たぶんifreqを初期化しているのだと思う。

     ioctl()の第二引数にSIOCGIFINDEXを指定することにより第三引数の情報を引き出すらしい。SIOCG**で取得用,
     SIOCS**で設定用らしい。第一引数でファイルディスクリプタを取得しているらしい。第三引数に関してだが、
     多分ifreq.ifr_nameあたりを見てnicから情報を引き出しsocの設定をおこなっていいるようだ。
     これ使えば自分でifconfigつくれるらしい。
     http://syuu1228.hatenablog.com/entry/20130706/1373137493


    bind()は例の如く成功して場合0を返すらしい。これを使うとソケットにIPとportをセットできるらしい。ただ今回はデータリンク層なので
    インターフェイス確定のために行っているらしい。
    
    */
    
    struct ifreq ifreq;
    struct sockaddr_ll sa;
    int soc,p;
   
    if(ip0nly){
        if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP)))<0){
            perror("socket");
            return(-1);
        }
    }else{
        if((soc=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL)))<0){
            
            perror("socket");
            return(-1);
        }
    
    }
    
    memset(&ifreq,0,sizeof(struct ifreq));
    
    strncpy(ifreq.ifr_name,device,sizeof(ifreq.ifr_name)-1);
    
    if(ioctl(soc,SIOCGIFINDEX,&ifreq)<0){
        perror("ioctl");
        close(soc);
        return -1;
    }
    
    //ここにsocket()の第一引数で指定したものと同じものを入れるのだと思う
    sa.sll_family=PF_PACKET;
    
    if(ip0nly){
        sa.sll_protocol=htons(ETH_P_IP);
    }else{
        sa.sll_protocol=htons(ETH_P_ALL);
    }

    sa.sll_ifindex=ifreq.ifr_ifindex;
    
    if(bind(soc,(struct sockaddr *)&sa,sizeof(sa))<0){
        
        perror("bind");
        close(soc);
        return -1;
    }
    
    if(promiscFlag){

        if(ioctl(soc,SIOCGIFFLAGS,&ifreq)<0){
            perror("ioctl");
            close(soc);
            return -1;
        }

        ifreq.ifr_flags=ifreq.ifr_flags;IFF_PROMISC;
        if(ioctl(soc,SIOCSIFFLAGS,&ifreq)<0){
            perror("ioctl");
            close(soc);
            return -1;
        }

    }

    return soc;
}

char *my_ether_ntoa_r(__u_char *hwaddr,char *buf,socklen_t size){

    snprintf(buf,size,"%02x:%02x:%02x:%02x:%02x:%02x",
    hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);

    return buf;
}

int PrintEtherHeader(struct ether_header *eh,FILE *fp){

    char buf[80];
    fprintf(fp,"ether_header-------------------------------\n");
    fprintf(fp,"ether_dhost=%s \n",my_ether_ntoa_r(eh->ether_dhost,buf,sizeof(buf)));//送信先
    fprintf(fp,"ether_shost=%s \n",my_ether_ntoa_r(eh->ether_shost,buf,sizeof(buf)));//送信元
    fprintf(fp,"ether_type=%02X\n",ntohs(eh->ether_type));
    switch(ntohs(eh->ether_type)){
        case ETH_P_IP:
            fprintf(fp,"(IP)\n");
            break;
        case ETH_P_IPV6:
            fprintf(fp,"(IPV6) \n");
            break;
        case ETH_P_ARP:
            fprintf(fp,"(ARP) \n");
            break;
        default:
            fprintf(fp,"(unknow) \n");
            break;
    }

    return 0;
}

int main(int argc,char *argv[],char *envp[]){


    int soc,size;
    __u_char buf[2048];
    
    if(argc<=1){
        fprintf(stderr,"ltest device-name \n");
        return -1;
    }
    
    if((soc=InitRawSocket(argv[1],0,0))==-1){
        fprintf(stderr,"InitRawsocket:error:%s\n",argv[1]);
        return -1;
    }
    
    while(1){
        if((size=read(soc,buf,sizeof(buf)))<=0){
            perror("read");
        }else{
            
            if(size>=sizeof(struct ether_header)){
                //bufは__u_charで取っているため、ここでキャストしている。
                PrintEtherHeader((struct ether_header *)buf,stdout);
            }
            else{
                fprintf(stderr,"read size(%d) < %d \n",size,sizeof(struct ether_header));
            }
        }

    }

    close(soc);

    return 0;
}