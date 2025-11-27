#include <iostream>
#include <cstring>
#include <thread>           
#include <chrono>          
#include <arpa/inet.h>      
#include <sys/socket.h>     
#include <linux/if_packet.h>
#include <net/ethernet.h> 
#include<net/if.h>
#include<unistd.h>  

#include "local.hpp"       
#include "arp.hpp"         

using namespace std;

int main() {
    
    const char* interface = "eth0"; 
    
    cout << "--- ARP Scanner Starting on " << interface << " ---" << endl;

   
    Localinfo my_info;
    try {
       
        getMACAddress(interface, my_info.src_mac);
        getSourceIP(interface, my_info.src_ip);
        getMask(interface, my_info.netmask);
        

        my_info.device.sll_family = AF_PACKET;
        my_info.device.sll_ifindex = if_nametoindex(interface);
        my_info.device.sll_halen = 6;
        memcpy(my_info.device.sll_addr, my_info.src_mac.data(), 6);

        cout << "My IP: " << inet_ntoa(my_info.src_ip.sin_addr) << endl;
        cout << "My MAC: ";
        for(auto b : my_info.src_mac) printf("%02x:", b);
        cout << endl;

    } catch (const exception& e) {
        cerr << "Failed to get local info: " << e.what() << endl;
        return -1;
    }

    
    int sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sd < 0) {
        perror("Socket creation failed");
        return -1;
    }

   
    map<array<uint8_t, 4>, array<uint8_t, 6>> ip_mac_pairs;

    
    cout << "[*] Starting Listener Thread..." << endl;
    thread listener_thread(rcvARPReply, sd, ref(ip_mac_pairs));
    listener_thread.detach(); 

    
    cout << "[*] Broadcasting ARP Requests..." << endl;
    
    sendARPRequest(sd, my_info);

    
    cout << "[*] Scanning complete. Waiting 5 seconds for late replies..." << endl;
    this_thread::sleep_for(chrono::seconds(5));

    cout << "\n--- Final Scan Results ---" << endl;
    cout << "Devices found: " << ip_mac_pairs.size() << endl;
    
    
    for (auto const& [ip, mac] : ip_mac_pairs) {
        printf("IP: %d.%d.%d.%d \t MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
            ip[0], ip[1], ip[2], ip[3],
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    close(sd);
    return 0;
}