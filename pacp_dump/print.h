char *my_ehter_ntoa_r(u_char *hwaddr,char *buf,socklen_t size);
char *arp_ip2str(u_int8_t *ip,char *buf,socklen_t size);
char *ip_ip2str(u_int32_t ip,char *buf,socklen_t size);
int PrintEtherHeader(struct ether_header *eh,FILE *fp);
int PrintArp(struct ether_arp *arp,FILE *fp);
int PrintIpHeader(struct iphdr *iphdr,u_char *option,int optionLen,FILE *fp);
int PrintIp6Header(struct ip6_hdr *ip6,FILE *fp);
int PrintIcmp(struct icmp *icmp,FILE *fp);
int PrintIcmp6(struct icmp6_hdr *icmp,FILE *fp);
int PrintTcp(struct tcphdr *tcphdr,FILE *fp);
int PrintUdp(struct udphdr *udphdr,FILE *fp);

int PrintIpHeader(struct iphdr *iphdr,__u_char *option,int optionLen,FILE *fp){
    int i;
    char buf[80];

    fprintf(fp,"ip-----------------------------------------------\n");
    fprintf(fp,"version=%u,",iphdr->version);
    fprintf(fp,"ihl=%u,",iphdr->ihl);
    fprintf(fp,"tos=%x,",iphdr->tos);
    fprintf(fp,"tot_len=%u,",ntohs(iphdr->tot_len));
    fprintf(fp,"id=%u\n",ntohs(iphdr->id));
    fprintf(fp,"frag_off)&0x1FFF=%x,%u,",(ntohs(iphdr->frag_off)>>13)&0x07,ntohs(iphdr->frag_off)&0x1FFF);
    fprintf(fp,"ttl=%u,",iphdr->protocol);
    fprintf(fp,"protocol=%u,"iphdr->protocol);
    if(iphdr->protocol<=17){
        fprintf(fp,"(%s),",Proto[iphdr->protocol]);
    }else{
        fprintf(fp,"(undefined),");
    }
    fprintf(fp,"check=%x\n",iphdr->check);
    fprintf(fp,"saddr=%s,",ip_ip2str(iphdr->saddr,buf,sizeof(buf)));
    fprintf(fp,"daddr=%s\n",ip_ip2str(iphdr->daddr,buf,sizeof(buf)));
    if(optionLen>0){
        fprintf(fp,"option:");
        for(i=0;i<optionLen;i++){
            if(i!=0){
                fprintf(fp,":%02x",option[i]);
            }else{
                fprintf(fp,"%02x",option[i]);
            }
        }
    }

    return 0;

}