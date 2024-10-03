// port_scanner.cpp
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

std::mutex print_mutex;
std::vector<int> open_ports;
int start_port, end_port;
std::string target_ip;

// Function to scan a single port
void scan_port(int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cerr << "[-] Could not create socket for port " << port << std::endl;
        return;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, target_ip.c_str(), &server_addr.sin_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    // Attempt to connect
    int result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if (result == 0) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[+] Port " << port << " is OPEN" << std::endl;
        open_ports.push_back(port);
    }

    close(sockfd);
}

// Function to start scanning
void start_scan() {
    std::vector<std::thread> threads;
    for (int port = start_port; port <= end_port; ++port) {
        threads.emplace_back(std::thread(scan_port, port));

        // Limit the number of concurrent threads
        if (threads.size() >= 100) {
            for (auto &th : threads) {
                th.join();
            }
            threads.clear();
        }
    }

    // Join any remaining threads
    for (auto &th : threads) {
        th.join();
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./port_scanner <target_ip> <start_port> <end_port>" << std::endl;
        return 1;
    }

    target_ip = argv[1];
    start_port = std::stoi(argv[2]);
    end_port = std::stoi(argv[3]);

    std::cout << "Starting port scan on " << target_ip << " from port " 
              << start_port << " to " << end_port << "..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    start_scan();
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "\nScan completed in " << duration.count() << " seconds." << std::endl;
    std::cout << "Open Ports:" << std::endl;
    for (const auto &port : open_ports) {
        std::cout << port << std::endl;
    }

    return 0;
}
