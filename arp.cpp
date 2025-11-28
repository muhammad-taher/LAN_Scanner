#include<bits/stdc++.h>
#include "arp.hpp"
#include<cstring>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<linux/if_packet.h>
#include<net/ethernet.h>
using namespace std;


void sendARPRequest(int sd,struct Localinfo &local_info){
    const int frameLength=ETH_HDRLEN+ARP_HDRLEN;
    uint8_t buffer[frameLength];

    memset(buffer,0xff,6);
    memcpy(buffer+6,local_info.src_mac.data(),6);
    buffer[12] = ETH_P_ARP / 256;
    buffer[13] = ETH_P_ARP % 256;
    struct arp_hdr arpHdr;
    arpHdr.htype=htons(1);
    arpHdr.ptype=htons(ETH_P_IP);
    arpHdr.hlen=6;
    arpHdr.plen=4;
    arpHdr.opcode=htons(ARPOP_REQUEST);

    arpHdr.sender_mac=local_info.src_mac;
    memcpy(arpHdr.sender_ip.data(),&local_info.src_ip.sin_addr.s_addr,4);
    arpHdr.target_mac.fill(0);

    uint32_t myIp=ntohl(local_info.src_ip.sin_addr.s_addr);
    uint32_t myMask=ntohl(local_info.netmask.sin_addr.s_addr);
    uint32_t netWorkPrefx=myIp&myMask;

    uint32_t numHosts=~myMask;

    struct sockaddr_ll dev=local_info.device;

    cout<<"Scanning "<<numHosts<<" Devices "<<endl;

    for(uint32_t i=1;i<numHosts;i++){
        uint32_t target_ip_int=netWorkPrefx | i;
        uint32_t target_ip_net_order=htonl(target_ip_int);

        memcpy(arpHdr.target_ip.data(),&target_ip_net_order,4);

        memcpy(buffer+ETH_HDRLEN,&arpHdr,ARP_HDRLEN);

        if((sendto(sd,buffer,frameLength,0,(struct sockaddr*)&dev,sizeof(dev)))<=0){
            perror("sendto() Failed");
        }
        usleep(50000);
    }

}

void parseARPReply(uint8_t* buffer, map<array<uint8_t, 4>, array<uint8_t, 6>> &ip_mac_pairs){
    struct arp_hdr* arp=(struct arp_hdr*)(buffer+ETH_HDRLEN);

    if(ntohs(arp->opcode)!=ARPOP_REPLY){
        perror("Not a Reply Packet");
        return;
    }

    ip_mac_pairs[arp->sender_ip]=arp->sender_mac;
    //printf("Found Device ! IP : %d.%d.%d.%d -> MAC : %02x.%02x.%02x.%02x.%02x.%02x\n",arp->sender_ip[0],arp->sender_ip[1],arp->sender_ip[2],arp->sender_ip[3],arp->sender_mac[0],arp->sender_mac[1],arp->sender_mac[2],arp->sender_mac[3],arp->sender_mac[4],arp->sender_mac[5]);

}

void rcvARPReply(int sd,map<array<uint8_t,4>,array<uint8_t,6>>&ip_mac_pairs){
    uint8_t buffer[65536];
    struct sockaddr saddr;
    socklen_t saddr_len=sizeof(saddr);
    cout<<"Listening for Reply ....."<<endl;
    while(true){
        int bytes=recvfrom(sd,buffer,sizeof(buffer),0,&saddr,&saddr_len);
        if(bytes<0){
            perror("recform() Failed");
            break;
        }
        uint16_t ethType=(buffer[12]<<8) | buffer[13];
        if(ethType==ETH_P_ARP){
            parseARPReply(buffer,ip_mac_pairs);
        }
    }
    

    
}
