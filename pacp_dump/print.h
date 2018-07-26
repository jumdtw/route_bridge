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

