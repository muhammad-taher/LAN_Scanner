Here’s a clean, professional, and GitHub-ready README for your ARP Network Scanner project:

---

# **Simple C++ ARP Network Scanner**

A lightweight, multi-threaded ARP network scanner built in C++ using raw sockets.
This tool broadcasts ARP requests across the local network and listens for responses to identify active devices — all without relying on external libraries.

---

## 🚀 **Features**

* **Raw Socket Implementation**
  Manually constructs Ethernet and ARP headers for precise low-level control.

* **Multi-Threaded Scanning**
  A dedicated listener thread captures ARP replies while the main thread broadcasts requests.

* **Low-Level System Interaction**
  Communicates directly with the Linux/Unix network stack using `ioctl`, `AF_PACKET`, and raw socket operations.

* **Cross-File Structure**
  Clean separation between packet creation, system info retrieval, and main logic.

---

## 📁 **Project Structure**

```
.
├── main.cpp        // Entry point: starts listener thread and triggers ARP broadcast
├── local.cpp       // Retrieves local IP, MAC address, and interface index
├── local.hpp
├── arp.cpp         // Constructs ARP packets and parses ARP replies
├── arp.hpp
└── README.md
```

---

## 🧰 **Prerequisites**

* **Linux or macOS**
  (Raw socket behavior varies by system; Linux recommended)

* **Root Privileges**
  Required for direct access to network interfaces.

* **G++ Compiler**
  With pthread support.

---

## 🔧 **Build Instructions**

Open your terminal inside the project folder and run:

```bash
g++ main.cpp local.cpp arp.cpp -o netscanner -pthread
```

---

## ▶️ **How to Run**

Since raw sockets require elevated privileges:

```bash
sudo ./netscanner
```

---

## ⚠️ **Disclaimer**

This tool is intended **strictly for educational and ethical security research**.
Do **NOT** scan networks without proper authorization.
The developer is not responsible for misuse of this software.

