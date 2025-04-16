#include "auth_comm.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include "socket_addr.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2)
  {
    std::cout << "Insufficient arguments.\nExpected: $./server <BROADCAST_IP_ADDR>\n";
  }
  
  sockaddr_in myAddr;
  myAddr.sin_family = AF_INET;
  myAddr.sin_port = htons( 80 );
  inet_pton(AF_INET, argv[1], &myAddr.sin_addr);

  AuthorizationServerComms comms("localhost:8000/authorize_login_node");
  comms.connect();
  std::cout << comms.get_status() << '\n';

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}
