int PrintfEtherHeader(char *device,int promiscFlag,int ip0nly){
    char buf[80];

    fprintf(fp,"ether_header--------------------------------\n");
    fprintf(fp,"ether_dhost=%s\n",my_ether_ntoa_r(eh->ether_dhost,buf,sizeof(buf)));
    fprintf(fp,"ether_shost=%s\n",my_ether_ntoa_r(eh->ether_shost,buf,sizeof(buf)));
    fprintf(fp,"ether_type=%02X",ntohs(eh->ether_type));
    switch(ntohs(eh->ether_type)){

        case ETH_P_IP:
            fprintf(fp,"(IP)\n");
            break;

        case ETH_P_IPV6:
            fprintf(fp,"(IPV6)\n");
            break;

        case ETH_P_ARP:
            fprintf(fp,"(unknow)\n");
            break;

    }
    return 0;
}