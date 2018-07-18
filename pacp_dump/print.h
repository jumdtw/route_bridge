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