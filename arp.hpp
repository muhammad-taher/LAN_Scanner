#ifndef ARP_HPP
#define ARP_HPP

#include<cstdint>
#include<array>
#include<map>
#include<linux/if_ether.h>
#include<net/if_arp.h>
#include "local.hpp"
using namespace std;

#define ETH_HDRLEN 14
#define ARP_HDRLEN 28


struct arp_hdr{
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t opcode;

    array<uint8_t,6>sender_mac;
    array<uint8_t,4>sender_ip;
    array<uint8_t,6>target_mac;
    array<uint8_t,4>target_ip;
};

void sendARPRequest(int sd, struct Localinfo &local_info);
void rcvARPReply(int sd,map<array<uint8_t,4>,array<uint8_t,6>>&ip_mac_pairs);
void parseARPReply(int sd,map<array<uint8_t,4>,array<uint8_t,6>>&ip_mac_pairs);

#endif