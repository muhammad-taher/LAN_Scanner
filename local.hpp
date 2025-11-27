#ifndef LOCAL_HPP
#define LOCAL_HPP

#include<netinet/in.h>
#include<linux/if_packet.h>
#include<array>
#include<cstdint>
using namespace std;


struct Localinfo{
    array<uint8_t,6>src_mac;
    struct sockaddr_in src_ip;
    struct sockaddr_in netmask;
    struct sockaddr_in gateway_ip;
    struct sockaddr_ll device;
};

void getMACAddress(const char *interface, array<uint8_t,6>&src_mac);
void getMask(const char *interface, struct sockaddr_in &netmask);
void getSourceIP(const char *interface, struct sockaddr_in &ipv4);
void getDefaultGateway(const char *interface, struct sockaddr_in &gateway_addr);

#endif


