#include "local.hpp"

#include <cstdio>       // For printf, fopen, perror
#include <cstdlib>      // For exit, EXIT_FAILURE
#include <cstring>      // For memset, memcpy, strcmp
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket(), AF_INET
#include <sys/ioctl.h>  // For ioctl(), SIOCGIFADDR
#include <net/if.h>     // For struct ifreq
#include <netinet/in.h> // For IPPROTO_RAW

void getDefaultGateway(const char *interface, struct sockaddr_in &gateway_addr){
    FILE *fp=fopen("/proc/net/route","r");
    if(fp==nullptr){
        perror("fopen Failed");
        exit(EXIT_FAILURE);
        
    }
    char line[100],iface[10];
    unsigned long dest,gateway;
    while(fgets(line,sizeof(line),fp)){
        if(sscanf(line,"%s\t%lX\t%lX",iface,&dest,&gateway)==3){
            if(strcmp(iface,interface)==0  && dest==0){
                gateway_addr.sin_family=AF_INET;
                gateway_addr.sin_addr.s_addr=gateway;
                break;
            }
        }
    }
    fclose(fp);
}

void getSourceIP(const char *interface, struct sockaddr_in &ipv4){
    int sd;
    struct ifreq ifr;

    if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("Socket() failed to open the socket descriptor!");
        exit(EXIT_FAILURE);
        
    }
    memset(&ifr,0,sizeof(ifr));
    snprintf(ifr.ifr_name,sizeof(ifr.ifr_name),"%s",interface);
    if(ioctl(sd,SIOCGIFADDR,&ifr)<0){
        perror("ioctl() Failed to get source ip");
        return;
    }

    memcpy(&ipv4,&ifr.ifr_addr,sizeof(sockaddr_in));

    close(sd);
}


void getMACAddress(const char *interface, array<uint8_t,6>&src_mac){
    int sd;
    struct ifreq ifr;

    if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("Socket() failed to open the socket descriptor!");
        exit(EXIT_FAILURE);
        
    }
    memset(&ifr,0,sizeof(ifr));
    snprintf(ifr.ifr_name,sizeof(ifr.ifr_name),"%s",interface);
    if(ioctl(sd,SIOCGIFHWADDR,&ifr)<0){
        perror("ioctl() Failed to get MAC Address");
        close(sd);
        return;
    }

    copy(ifr.ifr_hwaddr.sa_data,ifr.ifr_hwaddr.sa_data+6,src_mac.begin());

    close(sd);
}

void getMask(const char *interface, struct sockaddr_in &netmask){
    int sd;
    struct ifreq ifr;

    if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("Socket() failed to open the socket descriptor!");
        exit(EXIT_FAILURE);
        
    }
    memset(&ifr,0,sizeof(ifr));
    snprintf(ifr.ifr_name,sizeof(ifr.ifr_name),"%s",interface);
    if(ioctl(sd,SIOCGIFNETMASK,&ifr)<0){
        perror("ioctl() Failed to get source ip");
        return;
    }

    memcpy(&netmask,&ifr.ifr_netmask,sizeof(sockaddr_in));

    close(sd);
}

