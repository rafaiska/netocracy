#include "auth_comm.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include "user_comm.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2)
  {
    std::cout << "Insufficient arguments.\nExpected: $./server <BROADCAST_IP_ADDR>\n";
    return 1;
  }

  std::string address(argv[1]);
  ncy::UserCommunication user_comm(address);

  AuthorizationServerComms comms("localhost:8000/register_game_node");
  comms.connect();
  std::cout << comms.get_status() << '\n';

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}
