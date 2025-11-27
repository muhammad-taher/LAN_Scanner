#include <iostream>
#include <iomanip>      // For std::hex and std::setw
#include <cstring>      // For memset
#include <arpa/inet.h>  // For inet_ntoa (converts IP to String)
#include "local.hpp"    // Include your own header

using namespace std;

int main() {
    // 1. Define the interface you want to scan
    // TIP: Run "ip a" or "ifconfig" in your terminal to find your real name (e.g., wlan0, eth0)
    const char* interface_name = "eth0"; 

    cout << "--- Fetching Details for Interface: " << interface_name << " ---" << endl;

    // 2. Create the struct to hold the data
    Localinfo my_info;

    // 3. Call your functions to fill the struct
    // We need to verify these functions from local.cpp actually work!
    try {
        getSourceIP(interface_name, my_info.src_ip);
        getMACAddress(interface_name, my_info.src_mac);
        getMask(interface_name, my_info.netmask);
        getDefaultGateway(interface_name, my_info.gateway_ip);
    } catch (const exception& e) {
        cerr << "Error fetching data: " << e.what() << endl;
        return 1;
    }

    // 4. Print the Results
    
    // Print IP Address
    // inet_ntoa converts the raw binary IP into a string like "192.168.1.15"
    cout << "My IP Address:     " << inet_ntoa(my_info.src_ip.sin_addr) << endl;

    // Print Subnet Mask
    cout << "Subnet Mask:       " << inet_ntoa(my_info.netmask.sin_addr) << endl;

    // Print Gateway IP
    cout << "Gateway (Router):  " << inet_ntoa(my_info.gateway_ip.sin_addr) << endl;

    // Print MAC Address
    // We have to print this byte-by-byte in Hex format
    cout << "My MAC Address:    ";
    for (int i = 0; i < 6; i++) {
        // %02x means: Print 2 digits of Hex, add a leading zero if needed
        printf("%02x", my_info.src_mac[i]);
        if (i < 5) cout << ":";
    }
    cout << endl;

    return 0;
}